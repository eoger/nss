/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <functional>
#include <memory>
#include <vector>
#include "secerr.h"
#include "ssl.h"
#include "sslerr.h"
#include "sslproto.h"
#include "ssl3prot.h"

extern "C" {
// This is not something that should make you happy.
#include "libssl_internals.h"
}

#include "gtest_utils.h"
#include "scoped_ptrs.h"
#include "tls_connect.h"
#include "tls_filter.h"
#include "tls_parser.h"

namespace nss_test {

TEST_P(TlsConnectGeneric, SetupOnly) {}

TEST_P(TlsConnectGeneric, Connect) {
  SetExpectedVersion(std::get<1>(GetParam()));
  Connect();
  CheckKeys();
}

TEST_P(TlsConnectGeneric, ConnectEcdsa) {
  SetExpectedVersion(std::get<1>(GetParam()));
  Reset(TlsAgent::kServerEcdsa256);
  Connect();
  CheckKeys(ssl_kea_ecdh, ssl_auth_ecdsa);
}

TEST_P(TlsConnectGeneric, CipherSuiteMismatch) {
  EnsureTlsSetup();
  if (version_ >= SSL_LIBRARY_VERSION_TLS_1_3) {
    client_->EnableSingleCipher(TLS_AES_128_GCM_SHA256);
    server_->EnableSingleCipher(TLS_AES_256_GCM_SHA384);
  } else {
    client_->EnableSingleCipher(TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA);
    server_->EnableSingleCipher(TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA);
  }
  ConnectExpectAlert(server_, kTlsAlertHandshakeFailure);
  client_->CheckErrorCode(SSL_ERROR_NO_CYPHER_OVERLAP);
  server_->CheckErrorCode(SSL_ERROR_NO_CYPHER_OVERLAP);
}

class TlsAlertRecorder : public TlsRecordFilter {
 public:
  TlsAlertRecorder() : level_(255), description_(255) {}

  PacketFilter::Action FilterRecord(const TlsRecordHeader& header,
                                    const DataBuffer& input,
                                    DataBuffer* output) override {
    if (level_ != 255) {  // Already captured.
      return KEEP;
    }
    if (header.content_type() != kTlsAlertType) {
      return KEEP;
    }

    std::cerr << "Alert: " << input << std::endl;

    TlsParser parser(input);
    EXPECT_TRUE(parser.Read(&level_));
    EXPECT_TRUE(parser.Read(&description_));
    return KEEP;
  }

  uint8_t level() const { return level_; }
  uint8_t description() const { return description_; }

 private:
  uint8_t level_;
  uint8_t description_;
};

class HelloTruncator : public TlsHandshakeFilter {
  PacketFilter::Action FilterHandshake(const HandshakeHeader& header,
                                       const DataBuffer& input,
                                       DataBuffer* output) override {
    if (header.handshake_type() != kTlsHandshakeClientHello &&
        header.handshake_type() != kTlsHandshakeServerHello) {
      return KEEP;
    }
    output->Assign(input.data(), input.len() - 1);
    return CHANGE;
  }
};

// Verify that when NSS reports that an alert is sent, it is actually sent.
TEST_P(TlsConnectGeneric, CaptureAlertServer) {
  client_->SetPacketFilter(std::make_shared<HelloTruncator>());
  auto alert_recorder = std::make_shared<TlsAlertRecorder>();
  server_->SetPacketFilter(alert_recorder);

  ConnectExpectAlert(server_, kTlsAlertIllegalParameter);
  EXPECT_EQ(kTlsAlertFatal, alert_recorder->level());
  EXPECT_EQ(kTlsAlertIllegalParameter, alert_recorder->description());
}

TEST_P(TlsConnectGenericPre13, CaptureAlertClient) {
  server_->SetPacketFilter(std::make_shared<HelloTruncator>());
  auto alert_recorder = std::make_shared<TlsAlertRecorder>();
  client_->SetPacketFilter(alert_recorder);

  ConnectExpectAlert(client_, kTlsAlertDecodeError);
  EXPECT_EQ(kTlsAlertFatal, alert_recorder->level());
  EXPECT_EQ(kTlsAlertDecodeError, alert_recorder->description());
}

// In TLS 1.3, the server can't read the client alert.
TEST_P(TlsConnectTls13, CaptureAlertClient) {
  server_->SetPacketFilter(std::make_shared<HelloTruncator>());
  auto alert_recorder = std::make_shared<TlsAlertRecorder>();
  client_->SetPacketFilter(alert_recorder);

  server_->StartConnect();
  client_->StartConnect();

  client_->Handshake();
  client_->ExpectSendAlert(kTlsAlertDecodeError);
  server_->Handshake();
  client_->Handshake();
  if (variant_ == ssl_variant_stream) {
    // DTLS just drops the alert it can't decrypt.
    server_->ExpectSendAlert(kTlsAlertBadRecordMac);
  }
  server_->Handshake();
  EXPECT_EQ(kTlsAlertFatal, alert_recorder->level());
  EXPECT_EQ(kTlsAlertDecodeError, alert_recorder->description());
}

TEST_P(TlsConnectGenericPre13, ConnectFalseStart) {
  client_->EnableFalseStart();
  Connect();
  SendReceive();
}

TEST_P(TlsConnectGeneric, ConnectAlpn) {
  EnableAlpn();
  Connect();
  CheckAlpn("a");
}

TEST_P(TlsConnectGeneric, ConnectAlpnClone) {
  EnsureModelSockets();
  client_model_->EnableAlpn(alpn_dummy_val_, sizeof(alpn_dummy_val_));
  server_model_->EnableAlpn(alpn_dummy_val_, sizeof(alpn_dummy_val_));
  Connect();
  CheckAlpn("a");
}

TEST_P(TlsConnectDatagram, ConnectSrtp) {
  EnableSrtp();
  Connect();
  CheckSrtp();
  SendReceive();
}

// 1.3 is disabled in the next few tests because we don't
// presently support resumption in 1.3.
TEST_P(TlsConnectStreamPre13, ConnectAndClientRenegotiate) {
  Connect();
  server_->PrepareForRenegotiate();
  client_->StartRenegotiate();
  Handshake();
  CheckConnected();
}

TEST_P(TlsConnectStreamPre13, ConnectAndServerRenegotiate) {
  Connect();
  client_->PrepareForRenegotiate();
  server_->StartRenegotiate();
  Handshake();
  CheckConnected();
}

TEST_P(TlsConnectGeneric, ConnectSendReceive) {
  Connect();
  SendReceive();
}

class SaveTlsRecord : public TlsRecordFilter {
 public:
  SaveTlsRecord(size_t index) : index_(index), count_(0), contents_() {}

  const DataBuffer& contents() const { return contents_; }

 protected:
  PacketFilter::Action FilterRecord(const TlsRecordHeader& header,
                                    const DataBuffer& data,
                                    DataBuffer* changed) override {
    if (count_++ == index_) {
      contents_ = data;
    }
    return KEEP;
  }

 private:
  const size_t index_;
  size_t count_;
  DataBuffer contents_;
};

// Check that decrypting filters work and can read any record.
// This test (currently) only works in TLS 1.3 where we can decrypt.
TEST_F(TlsConnectStreamTls13, DecryptRecordClient) {
  EnsureTlsSetup();
  // 0 = ClientHello, 1 = Finished, 2 = SendReceive, 3 = SendBuffer
  auto saved = std::make_shared<SaveTlsRecord>(3);
  client_->SetTlsRecordFilter(saved);
  Connect();
  SendReceive();

  static const uint8_t data[] = {0xde, 0xad, 0xdc};
  DataBuffer buf(data, sizeof(data));
  client_->SendBuffer(buf);
  EXPECT_EQ(buf, saved->contents());
}

TEST_F(TlsConnectStreamTls13, DecryptRecordServer) {
  EnsureTlsSetup();
  // Disable tickets so that we are sure to not get NewSessionTicket.
  EXPECT_EQ(SECSuccess, SSL_OptionSet(server_->ssl_fd(),
                                      SSL_ENABLE_SESSION_TICKETS, PR_FALSE));
  // 0 = ServerHello, 1 = other handshake, 2 = SendReceive, 3 = SendBuffer
  auto saved = std::make_shared<SaveTlsRecord>(3);
  server_->SetTlsRecordFilter(saved);
  Connect();
  SendReceive();

  static const uint8_t data[] = {0xde, 0xad, 0xd5};
  DataBuffer buf(data, sizeof(data));
  server_->SendBuffer(buf);
  EXPECT_EQ(buf, saved->contents());
}

class DropTlsRecord : public TlsRecordFilter {
 public:
  DropTlsRecord(size_t index) : index_(index), count_(0) {}

 protected:
  PacketFilter::Action FilterRecord(const TlsRecordHeader& header,
                                    const DataBuffer& data,
                                    DataBuffer* changed) override {
    if (count_++ == index_) {
      return DROP;
    }
    return KEEP;
  }

 private:
  const size_t index_;
  size_t count_;
};

// Test that decrypting filters work correctly and are able to drop records.
TEST_F(TlsConnectStreamTls13, DropRecordServer) {
  EnsureTlsSetup();
  // Disable session tickets so that the server doesn't send an extra record.
  EXPECT_EQ(SECSuccess, SSL_OptionSet(server_->ssl_fd(),
                                      SSL_ENABLE_SESSION_TICKETS, PR_FALSE));

  // 0 = ServerHello, 1 = other handshake, 2 = first write
  server_->SetTlsRecordFilter(std::make_shared<DropTlsRecord>(2));
  Connect();
  server_->SendData(23, 23);  // This should be dropped, so it won't be counted.
  server_->ResetSentBytes();
  SendReceive();
}

TEST_F(TlsConnectStreamTls13, DropRecordClient) {
  EnsureTlsSetup();
  // 0 = ClientHello, 1 = Finished, 2 = first write
  client_->SetTlsRecordFilter(std::make_shared<DropTlsRecord>(2));
  Connect();
  client_->SendData(26, 26);  // This should be dropped, so it won't be counted.
  client_->ResetSentBytes();
  SendReceive();
}

// The next two tests takes advantage of the fact that we
// automatically read the first 1024 bytes, so if
// we provide 1200 bytes, they overrun the read buffer
// provided by the calling test.

// DTLS should return an error.
TEST_P(TlsConnectDatagram, ShortRead) {
  Connect();
  client_->ExpectReadWriteError();
  server_->SendData(50, 50);
  client_->ReadBytes(20);
  EXPECT_EQ(0U, client_->received_bytes());
  EXPECT_EQ(SSL_ERROR_RX_SHORT_DTLS_READ, PORT_GetError());

  // Now send and receive another packet.
  server_->ResetSentBytes();  // Reset the counter.
  SendReceive();
}

// TLS should get the write in two chunks.
TEST_P(TlsConnectStream, ShortRead) {
  // This test behaves oddly with TLS 1.0 because of 1/n+1 splitting,
  // so skip in that case.
  if (version_ < SSL_LIBRARY_VERSION_TLS_1_1) return;

  Connect();
  server_->SendData(50, 50);
  // Read the first tranche.
  client_->ReadBytes(20);
  ASSERT_EQ(20U, client_->received_bytes());
  // The second tranche should now immediately be available.
  client_->ReadBytes();
  ASSERT_EQ(50U, client_->received_bytes());
}

TEST_P(TlsConnectGeneric, ConnectWithCompressionMaybe) {
  EnsureTlsSetup();
  client_->EnableCompression();
  server_->EnableCompression();
  Connect();
  EXPECT_EQ(client_->version() < SSL_LIBRARY_VERSION_TLS_1_3 &&
                variant_ != ssl_variant_datagram,
            client_->is_compressed());
  SendReceive();
}

TEST_P(TlsConnectDatagram, TestDtlsHolddownExpiry) {
  Connect();
  std::cerr << "Expiring holddown timer\n";
  SSLInt_ForceTimerExpiry(client_->ssl_fd());
  SSLInt_ForceTimerExpiry(server_->ssl_fd());
  SendReceive();
  if (version_ >= SSL_LIBRARY_VERSION_TLS_1_3) {
    // One for send, one for receive.
    EXPECT_EQ(2, SSLInt_CountTls13CipherSpecs(client_->ssl_fd()));
  }
}

class TlsPreCCSHeaderInjector : public TlsRecordFilter {
 public:
  TlsPreCCSHeaderInjector() {}
  virtual PacketFilter::Action FilterRecord(
      const TlsRecordHeader& record_header, const DataBuffer& input,
      size_t* offset, DataBuffer* output) override {
    if (record_header.content_type() != kTlsChangeCipherSpecType) return KEEP;

    std::cerr << "Injecting Finished header before CCS\n";
    const uint8_t hhdr[] = {kTlsHandshakeFinished, 0x00, 0x00, 0x0c};
    DataBuffer hhdr_buf(hhdr, sizeof(hhdr));
    TlsRecordHeader nhdr(record_header.version(), kTlsHandshakeType, 0);
    *offset = nhdr.Write(output, *offset, hhdr_buf);
    *offset = record_header.Write(output, *offset, input);
    return CHANGE;
  }
};

TEST_P(TlsConnectStreamPre13, ClientFinishedHeaderBeforeCCS) {
  client_->SetPacketFilter(std::make_shared<TlsPreCCSHeaderInjector>());
  ConnectExpectAlert(server_, kTlsAlertUnexpectedMessage);
  client_->CheckErrorCode(SSL_ERROR_HANDSHAKE_UNEXPECTED_ALERT);
  server_->CheckErrorCode(SSL_ERROR_RX_UNEXPECTED_CHANGE_CIPHER);
}

TEST_P(TlsConnectStreamPre13, ServerFinishedHeaderBeforeCCS) {
  server_->SetPacketFilter(std::make_shared<TlsPreCCSHeaderInjector>());
  client_->StartConnect();
  server_->StartConnect();
  ExpectAlert(client_, kTlsAlertUnexpectedMessage);
  Handshake();
  EXPECT_EQ(TlsAgent::STATE_ERROR, client_->state());
  client_->CheckErrorCode(SSL_ERROR_RX_UNEXPECTED_CHANGE_CIPHER);
  EXPECT_EQ(TlsAgent::STATE_CONNECTED, server_->state());
  server_->Handshake();  // Make sure alert is consumed.
}

TEST_P(TlsConnectTls13, UnknownAlert) {
  Connect();
  server_->ExpectSendAlert(0xff, kTlsAlertWarning);
  client_->ExpectReceiveAlert(0xff, kTlsAlertWarning);
  SSLInt_SendAlert(server_->ssl_fd(), kTlsAlertWarning,
                   0xff);  // Unknown value.
  client_->ExpectReadWriteError();
  client_->WaitForErrorCode(SSL_ERROR_RX_UNKNOWN_ALERT, 2000);
}

TEST_P(TlsConnectTls13, AlertWrongLevel) {
  Connect();
  server_->ExpectSendAlert(kTlsAlertUnexpectedMessage, kTlsAlertWarning);
  client_->ExpectReceiveAlert(kTlsAlertUnexpectedMessage, kTlsAlertWarning);
  SSLInt_SendAlert(server_->ssl_fd(), kTlsAlertWarning,
                   kTlsAlertUnexpectedMessage);
  client_->ExpectReadWriteError();
  client_->WaitForErrorCode(SSL_ERROR_HANDSHAKE_UNEXPECTED_ALERT, 2000);
}

TEST_F(TlsConnectStreamTls13, Tls13FailedWriteSecondFlight) {
  EnsureTlsSetup();
  client_->StartConnect();
  server_->StartConnect();
  client_->Handshake();
  server_->Handshake();  // Send first flight.
  client_->adapter()->CloseWrites();
  client_->Handshake();  // This will get an error, but shouldn't crash.
  client_->CheckErrorCode(SSL_ERROR_SOCKET_WRITE_FAILURE);
}

TEST_F(TlsConnectTest, ConnectSSLv3) {
  ConfigureVersion(SSL_LIBRARY_VERSION_3_0);
  EnableOnlyStaticRsaCiphers();
  Connect();
  CheckKeys(ssl_kea_rsa, ssl_grp_none, ssl_auth_rsa_decrypt, ssl_sig_none);
}

TEST_F(TlsConnectTest, ConnectSSLv3ClientAuth) {
  ConfigureVersion(SSL_LIBRARY_VERSION_3_0);
  EnableOnlyStaticRsaCiphers();
  client_->SetupClientAuth();
  server_->RequestClientAuth(true);
  Connect();
  CheckKeys(ssl_kea_rsa, ssl_grp_none, ssl_auth_rsa_decrypt, ssl_sig_none);
}

TEST_F(TlsConnectStreamTls13, ClientAltHandshakeType) {
  client_->SetAltHandshakeTypeEnabled();
  auto filter = std::make_shared<TlsHeaderRecorder>();
  server_->SetPacketFilter(filter);
  Connect();
  ASSERT_EQ(kTlsHandshakeType, filter->header(0)->content_type());
}

TEST_F(TlsConnectStreamTls13, ServerAltHandshakeType) {
  server_->SetAltHandshakeTypeEnabled();
  auto filter = std::make_shared<TlsHeaderRecorder>();
  server_->SetPacketFilter(filter);
  Connect();
  ASSERT_EQ(kTlsHandshakeType, filter->header(0)->content_type());
}

TEST_F(TlsConnectStreamTls13, BothAltHandshakeType) {
  client_->SetAltHandshakeTypeEnabled();
  server_->SetAltHandshakeTypeEnabled();
  auto header_filter = std::make_shared<TlsHeaderRecorder>();
  auto sh_filter = std::make_shared<TlsInspectorRecordHandshakeMessage>(
      kTlsHandshakeServerHello);
  std::vector<std::shared_ptr<PacketFilter>> filters = {header_filter,
                                                        sh_filter};
  auto chained = std::make_shared<ChainedPacketFilter>(filters);
  server_->SetPacketFilter(chained);
  header_filter->SetAgent(server_.get());
  header_filter->EnableDecryption();
  Connect();
  ASSERT_EQ(kTlsAltHandshakeType, header_filter->header(0)->content_type());
  ASSERT_EQ(kTlsHandshakeType, header_filter->header(1)->content_type());
  uint32_t ver;
  ASSERT_TRUE(sh_filter->buffer().Read(0, 2, &ver));
  ASSERT_EQ((uint32_t)(0x7a00 | TLS_1_3_DRAFT_VERSION), ver);
}

INSTANTIATE_TEST_CASE_P(
    GenericStream, TlsConnectGeneric,
    ::testing::Combine(TlsConnectTestBase::kTlsVariantsStream,
                       TlsConnectTestBase::kTlsVAll));
INSTANTIATE_TEST_CASE_P(
    GenericDatagram, TlsConnectGeneric,
    ::testing::Combine(TlsConnectTestBase::kTlsVariantsDatagram,
                       TlsConnectTestBase::kTlsV11Plus));

INSTANTIATE_TEST_CASE_P(StreamOnly, TlsConnectStream,
                        TlsConnectTestBase::kTlsVAll);
INSTANTIATE_TEST_CASE_P(DatagramOnly, TlsConnectDatagram,
                        TlsConnectTestBase::kTlsV11Plus);

INSTANTIATE_TEST_CASE_P(
    Pre12Stream, TlsConnectPre12,
    ::testing::Combine(TlsConnectTestBase::kTlsVariantsStream,
                       TlsConnectTestBase::kTlsV10V11));
INSTANTIATE_TEST_CASE_P(
    Pre12Datagram, TlsConnectPre12,
    ::testing::Combine(TlsConnectTestBase::kTlsVariantsDatagram,
                       TlsConnectTestBase::kTlsV11));

INSTANTIATE_TEST_CASE_P(Version12Only, TlsConnectTls12,
                        TlsConnectTestBase::kTlsVariantsAll);
#ifndef NSS_DISABLE_TLS_1_3
INSTANTIATE_TEST_CASE_P(Version13Only, TlsConnectTls13,
                        TlsConnectTestBase::kTlsVariantsAll);
#endif

INSTANTIATE_TEST_CASE_P(
    Pre13Stream, TlsConnectGenericPre13,
    ::testing::Combine(TlsConnectTestBase::kTlsVariantsStream,
                       TlsConnectTestBase::kTlsV10ToV12));
INSTANTIATE_TEST_CASE_P(
    Pre13Datagram, TlsConnectGenericPre13,
    ::testing::Combine(TlsConnectTestBase::kTlsVariantsDatagram,
                       TlsConnectTestBase::kTlsV11V12));
INSTANTIATE_TEST_CASE_P(Pre13StreamOnly, TlsConnectStreamPre13,
                        TlsConnectTestBase::kTlsV10ToV12);

INSTANTIATE_TEST_CASE_P(Version12Plus, TlsConnectTls12Plus,
                        ::testing::Combine(TlsConnectTestBase::kTlsVariantsAll,
                                           TlsConnectTestBase::kTlsV12Plus));

}  // namespace nspr_test
