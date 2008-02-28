/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape security libraries.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1994-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Dr Vipul Gupta <vipul.gupta@sun.com>, Sun Microsystems Laboratories
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef _SECOIDT_H_
#define _SECOIDT_H_

#include "utilrename.h"

/*
 * secoidt.h - public data structures for ASN.1 OID functions
 *
 * $Id$
 */

#include "secitem.h"

typedef struct SECOidDataStr SECOidData;
typedef struct SECAlgorithmIDStr SECAlgorithmID;

/*
** An X.500 algorithm identifier
*/
struct SECAlgorithmIDStr {
    SECItem algorithm;
    SECItem parameters;
};

/*
 * Misc object IDs - these numbers are for convenient handling.
 * They are mapped into real object IDs
 *
 * NOTE: the order of these entries must mach the array "oids" of SECOidData
 * in util/secoid.c.
 */
typedef enum {
    SEC_OID_UNKNOWN = 0,
    SEC_OID_MD2 = 1,
    SEC_OID_MD4 = 2,
    SEC_OID_MD5 = 3,
    SEC_OID_SHA1 = 4,
    SEC_OID_RC2_CBC = 5,
    SEC_OID_RC4 = 6,
    SEC_OID_DES_EDE3_CBC = 7,
    SEC_OID_RC5_CBC_PAD = 8,
    SEC_OID_DES_ECB = 9,
    SEC_OID_DES_CBC = 10,
    SEC_OID_DES_OFB = 11,
    SEC_OID_DES_CFB = 12,
    SEC_OID_DES_MAC = 13,
    SEC_OID_DES_EDE = 14,
    SEC_OID_ISO_SHA_WITH_RSA_SIGNATURE = 15,
    SEC_OID_PKCS1_RSA_ENCRYPTION = 16,
    SEC_OID_PKCS1_MD2_WITH_RSA_ENCRYPTION = 17,
    SEC_OID_PKCS1_MD4_WITH_RSA_ENCRYPTION = 18,
    SEC_OID_PKCS1_MD5_WITH_RSA_ENCRYPTION = 19,
    SEC_OID_PKCS1_SHA1_WITH_RSA_ENCRYPTION = 20,
    SEC_OID_PKCS5_PBE_WITH_MD2_AND_DES_CBC = 21,
    SEC_OID_PKCS5_PBE_WITH_MD5_AND_DES_CBC = 22,
    SEC_OID_PKCS5_PBE_WITH_SHA1_AND_DES_CBC = 23,
    SEC_OID_PKCS7 = 24,
    SEC_OID_PKCS7_DATA = 25,
    SEC_OID_PKCS7_SIGNED_DATA = 26,
    SEC_OID_PKCS7_ENVELOPED_DATA = 27,
    SEC_OID_PKCS7_SIGNED_ENVELOPED_DATA = 28,
    SEC_OID_PKCS7_DIGESTED_DATA = 29,
    SEC_OID_PKCS7_ENCRYPTED_DATA = 30,
    SEC_OID_PKCS9_EMAIL_ADDRESS = 31,
    SEC_OID_PKCS9_UNSTRUCTURED_NAME = 32,
    SEC_OID_PKCS9_CONTENT_TYPE = 33,
    SEC_OID_PKCS9_MESSAGE_DIGEST = 34,
    SEC_OID_PKCS9_SIGNING_TIME = 35,
    SEC_OID_PKCS9_COUNTER_SIGNATURE = 36,
    SEC_OID_PKCS9_CHALLENGE_PASSWORD = 37,
    SEC_OID_PKCS9_UNSTRUCTURED_ADDRESS = 38,
    SEC_OID_PKCS9_EXTENDED_CERTIFICATE_ATTRIBUTES = 39,
    SEC_OID_PKCS9_SMIME_CAPABILITIES = 40,
    SEC_OID_AVA_COMMON_NAME = 41,
    SEC_OID_AVA_COUNTRY_NAME = 42,
    SEC_OID_AVA_LOCALITY = 43,
    SEC_OID_AVA_STATE_OR_PROVINCE = 44,
    SEC_OID_AVA_ORGANIZATION_NAME = 45,
    SEC_OID_AVA_ORGANIZATIONAL_UNIT_NAME = 46,
    SEC_OID_AVA_DN_QUALIFIER = 47,
    SEC_OID_AVA_DC = 48,

    SEC_OID_NS_TYPE_GIF = 49,
    SEC_OID_NS_TYPE_JPEG = 50,
    SEC_OID_NS_TYPE_URL = 51,
    SEC_OID_NS_TYPE_HTML = 52,
    SEC_OID_NS_TYPE_CERT_SEQUENCE = 53,
    SEC_OID_MISSI_KEA_DSS_OLD = 54,
    SEC_OID_MISSI_DSS_OLD = 55,
    SEC_OID_MISSI_KEA_DSS = 56,
    SEC_OID_MISSI_DSS = 57,
    SEC_OID_MISSI_KEA = 58,
    SEC_OID_MISSI_ALT_KEA = 59,

    /* Netscape private certificate extensions */
    SEC_OID_NS_CERT_EXT_NETSCAPE_OK = 60,
    SEC_OID_NS_CERT_EXT_ISSUER_LOGO = 61,
    SEC_OID_NS_CERT_EXT_SUBJECT_LOGO = 62,
    SEC_OID_NS_CERT_EXT_CERT_TYPE = 63,
    SEC_OID_NS_CERT_EXT_BASE_URL = 64,
    SEC_OID_NS_CERT_EXT_REVOCATION_URL = 65,
    SEC_OID_NS_CERT_EXT_CA_REVOCATION_URL = 66,
    SEC_OID_NS_CERT_EXT_CA_CRL_URL = 67,
    SEC_OID_NS_CERT_EXT_CA_CERT_URL = 68,
    SEC_OID_NS_CERT_EXT_CERT_RENEWAL_URL = 69,
    SEC_OID_NS_CERT_EXT_CA_POLICY_URL = 70,
    SEC_OID_NS_CERT_EXT_HOMEPAGE_URL = 71,
    SEC_OID_NS_CERT_EXT_ENTITY_LOGO = 72,
    SEC_OID_NS_CERT_EXT_USER_PICTURE = 73,
    SEC_OID_NS_CERT_EXT_SSL_SERVER_NAME = 74,
    SEC_OID_NS_CERT_EXT_COMMENT = 75,
    SEC_OID_NS_CERT_EXT_LOST_PASSWORD_URL = 76,
    SEC_OID_NS_CERT_EXT_CERT_RENEWAL_TIME = 77,
    SEC_OID_NS_KEY_USAGE_GOVT_APPROVED = 78,

    /* x.509 v3 Extensions */
    SEC_OID_X509_SUBJECT_DIRECTORY_ATTR = 79,
    SEC_OID_X509_SUBJECT_KEY_ID = 80,
    SEC_OID_X509_KEY_USAGE = 81,
    SEC_OID_X509_PRIVATE_KEY_USAGE_PERIOD = 82,
    SEC_OID_X509_SUBJECT_ALT_NAME = 83,
    SEC_OID_X509_ISSUER_ALT_NAME = 84,
    SEC_OID_X509_BASIC_CONSTRAINTS = 85,
    SEC_OID_X509_NAME_CONSTRAINTS = 86,
    SEC_OID_X509_CRL_DIST_POINTS = 87,
    SEC_OID_X509_CERTIFICATE_POLICIES = 88,
    SEC_OID_X509_POLICY_MAPPINGS = 89,
    SEC_OID_X509_POLICY_CONSTRAINTS = 90,
    SEC_OID_X509_AUTH_KEY_ID = 91,
    SEC_OID_X509_EXT_KEY_USAGE = 92,
    SEC_OID_X509_AUTH_INFO_ACCESS = 93,

    SEC_OID_X509_CRL_NUMBER = 94,
    SEC_OID_X509_REASON_CODE = 95,
    SEC_OID_X509_INVALID_DATE = 96,
    /* End of x.509 v3 Extensions */    

    SEC_OID_X500_RSA_ENCRYPTION = 97,

    /* alg 1485 additions */
    SEC_OID_RFC1274_UID = 98,
    SEC_OID_RFC1274_MAIL = 99,

    /* PKCS 12 additions */
    SEC_OID_PKCS12 = 100,
    SEC_OID_PKCS12_MODE_IDS = 101,
    SEC_OID_PKCS12_ESPVK_IDS = 102,
    SEC_OID_PKCS12_BAG_IDS = 103,
    SEC_OID_PKCS12_CERT_BAG_IDS = 104,
    SEC_OID_PKCS12_OIDS = 105,
    SEC_OID_PKCS12_PBE_IDS = 106,
    SEC_OID_PKCS12_SIGNATURE_IDS = 107,
    SEC_OID_PKCS12_ENVELOPING_IDS = 108,
   /* SEC_OID_PKCS12_OFFLINE_TRANSPORT_MODE,
    SEC_OID_PKCS12_ONLINE_TRANSPORT_MODE, */
    SEC_OID_PKCS12_PKCS8_KEY_SHROUDING = 109,
    SEC_OID_PKCS12_KEY_BAG_ID = 110,
    SEC_OID_PKCS12_CERT_AND_CRL_BAG_ID = 111,
    SEC_OID_PKCS12_SECRET_BAG_ID = 112,
    SEC_OID_PKCS12_X509_CERT_CRL_BAG = 113,
    SEC_OID_PKCS12_SDSI_CERT_BAG = 114,
    SEC_OID_PKCS12_PBE_WITH_SHA1_AND_128_BIT_RC4 = 115,
    SEC_OID_PKCS12_PBE_WITH_SHA1_AND_40_BIT_RC4 = 116,
    SEC_OID_PKCS12_PBE_WITH_SHA1_AND_TRIPLE_DES_CBC = 117,
    SEC_OID_PKCS12_PBE_WITH_SHA1_AND_128_BIT_RC2_CBC = 118,
    SEC_OID_PKCS12_PBE_WITH_SHA1_AND_40_BIT_RC2_CBC = 119,
    SEC_OID_PKCS12_RSA_ENCRYPTION_WITH_128_BIT_RC4 = 120,
    SEC_OID_PKCS12_RSA_ENCRYPTION_WITH_40_BIT_RC4 = 121,
    SEC_OID_PKCS12_RSA_ENCRYPTION_WITH_TRIPLE_DES = 122,
    SEC_OID_PKCS12_RSA_SIGNATURE_WITH_SHA1_DIGEST = 123,
    /* end of PKCS 12 additions */

    /* DSA signatures */
    SEC_OID_ANSIX9_DSA_SIGNATURE = 124,
    SEC_OID_ANSIX9_DSA_SIGNATURE_WITH_SHA1_DIGEST = 125,
    SEC_OID_BOGUS_DSA_SIGNATURE_WITH_SHA1_DIGEST = 126,

    /* Verisign OIDs */
    SEC_OID_VERISIGN_USER_NOTICES = 127,

    /* PKIX OIDs */
    SEC_OID_PKIX_CPS_POINTER_QUALIFIER = 128,
    SEC_OID_PKIX_USER_NOTICE_QUALIFIER = 129,
    SEC_OID_PKIX_OCSP = 130,
    SEC_OID_PKIX_OCSP_BASIC_RESPONSE = 131,
    SEC_OID_PKIX_OCSP_NONCE = 132,
    SEC_OID_PKIX_OCSP_CRL = 133,
    SEC_OID_PKIX_OCSP_RESPONSE = 134,
    SEC_OID_PKIX_OCSP_NO_CHECK = 135,
    SEC_OID_PKIX_OCSP_ARCHIVE_CUTOFF = 136,
    SEC_OID_PKIX_OCSP_SERVICE_LOCATOR = 137,
    SEC_OID_PKIX_REGCTRL_REGTOKEN = 138,
    SEC_OID_PKIX_REGCTRL_AUTHENTICATOR = 139,
    SEC_OID_PKIX_REGCTRL_PKIPUBINFO = 140,
    SEC_OID_PKIX_REGCTRL_PKI_ARCH_OPTIONS = 141,
    SEC_OID_PKIX_REGCTRL_OLD_CERT_ID = 142,
    SEC_OID_PKIX_REGCTRL_PROTOCOL_ENC_KEY = 143,
    SEC_OID_PKIX_REGINFO_UTF8_PAIRS = 144,
    SEC_OID_PKIX_REGINFO_CERT_REQUEST = 145,
    SEC_OID_EXT_KEY_USAGE_SERVER_AUTH = 146,
    SEC_OID_EXT_KEY_USAGE_CLIENT_AUTH = 147,
    SEC_OID_EXT_KEY_USAGE_CODE_SIGN = 148,
    SEC_OID_EXT_KEY_USAGE_EMAIL_PROTECT = 149,
    SEC_OID_EXT_KEY_USAGE_TIME_STAMP = 150,
    SEC_OID_OCSP_RESPONDER = 151,

    /* Netscape Algorithm OIDs */
    SEC_OID_NETSCAPE_SMIME_KEA = 152,

    /* Skipjack OID -- ### mwelch temporary */
    SEC_OID_FORTEZZA_SKIPJACK = 153,

    /* PKCS 12 V2 oids */
    SEC_OID_PKCS12_V2_PBE_WITH_SHA1_AND_128_BIT_RC4 = 154,
    SEC_OID_PKCS12_V2_PBE_WITH_SHA1_AND_40_BIT_RC4 = 155,
    SEC_OID_PKCS12_V2_PBE_WITH_SHA1_AND_3KEY_TRIPLE_DES_CBC = 156,
    SEC_OID_PKCS12_V2_PBE_WITH_SHA1_AND_2KEY_TRIPLE_DES_CBC = 157,
    SEC_OID_PKCS12_V2_PBE_WITH_SHA1_AND_128_BIT_RC2_CBC = 158,
    SEC_OID_PKCS12_V2_PBE_WITH_SHA1_AND_40_BIT_RC2_CBC = 159,
    SEC_OID_PKCS12_SAFE_CONTENTS_ID = 160,
    SEC_OID_PKCS12_PKCS8_SHROUDED_KEY_BAG_ID = 161,

    SEC_OID_PKCS12_V1_KEY_BAG_ID = 162,
    SEC_OID_PKCS12_V1_PKCS8_SHROUDED_KEY_BAG_ID = 163,
    SEC_OID_PKCS12_V1_CERT_BAG_ID = 164,
    SEC_OID_PKCS12_V1_CRL_BAG_ID = 165,
    SEC_OID_PKCS12_V1_SECRET_BAG_ID = 166,
    SEC_OID_PKCS12_V1_SAFE_CONTENTS_BAG_ID = 167,
    SEC_OID_PKCS9_X509_CERT = 168,
    SEC_OID_PKCS9_SDSI_CERT = 169,
    SEC_OID_PKCS9_X509_CRL = 170,
    SEC_OID_PKCS9_FRIENDLY_NAME = 171,
    SEC_OID_PKCS9_LOCAL_KEY_ID = 172,
    SEC_OID_BOGUS_KEY_USAGE = 173,

    /*Diffe Helman OIDS */
    SEC_OID_X942_DIFFIE_HELMAN_KEY = 174,

    /* Netscape other name types */
    SEC_OID_NETSCAPE_NICKNAME = 175,

    /* Cert Server OIDS */
    SEC_OID_NETSCAPE_RECOVERY_REQUEST = 176,

    /* New PSM certificate management OIDs */
    SEC_OID_CERT_RENEWAL_LOCATOR = 177,
    SEC_OID_NS_CERT_EXT_SCOPE_OF_USE = 178,
    
    /* CMS (RFC2630) OIDs */
    SEC_OID_CMS_EPHEMERAL_STATIC_DIFFIE_HELLMAN = 179,
    SEC_OID_CMS_3DES_KEY_WRAP = 180,
    SEC_OID_CMS_RC2_KEY_WRAP = 181,

    /* SMIME attributes */
    SEC_OID_SMIME_ENCRYPTION_KEY_PREFERENCE = 182,

    /* AES OIDs */
    SEC_OID_AES_128_ECB 	= 183,
    SEC_OID_AES_128_CBC 	= 184,
    SEC_OID_AES_192_ECB 	= 185,
    SEC_OID_AES_192_CBC 	= 186,
    SEC_OID_AES_256_ECB 	= 187,
    SEC_OID_AES_256_CBC 	= 188,

    SEC_OID_SDN702_DSA_SIGNATURE = 189,

    SEC_OID_MS_SMIME_ENCRYPTION_KEY_PREFERENCE = 190,

    SEC_OID_SHA256              = 191,
    SEC_OID_SHA384              = 192,
    SEC_OID_SHA512              = 193,

    SEC_OID_PKCS1_SHA256_WITH_RSA_ENCRYPTION = 194,
    SEC_OID_PKCS1_SHA384_WITH_RSA_ENCRYPTION = 195,
    SEC_OID_PKCS1_SHA512_WITH_RSA_ENCRYPTION = 196,

    SEC_OID_AES_128_KEY_WRAP	= 197,
    SEC_OID_AES_192_KEY_WRAP	= 198,
    SEC_OID_AES_256_KEY_WRAP	= 199,

    /* Elliptic Curve Cryptography (ECC) OIDs */
    SEC_OID_ANSIX962_EC_PUBLIC_KEY  = 200,
    SEC_OID_ANSIX962_ECDSA_SHA1_SIGNATURE = 201,

#define SEC_OID_ANSIX962_ECDSA_SIGNATURE_WITH_SHA1_DIGEST \
	SEC_OID_ANSIX962_ECDSA_SHA1_SIGNATURE

    /* ANSI X9.62 named elliptic curves (prime field) */
    SEC_OID_ANSIX962_EC_PRIME192V1  = 202,
    SEC_OID_ANSIX962_EC_PRIME192V2  = 203,
    SEC_OID_ANSIX962_EC_PRIME192V3  = 204,
    SEC_OID_ANSIX962_EC_PRIME239V1  = 205,
    SEC_OID_ANSIX962_EC_PRIME239V2  = 206,
    SEC_OID_ANSIX962_EC_PRIME239V3  = 207,
    SEC_OID_ANSIX962_EC_PRIME256V1  = 208,

    /* SECG named elliptic curves (prime field) */
    SEC_OID_SECG_EC_SECP112R1       = 209,
    SEC_OID_SECG_EC_SECP112R2       = 210,
    SEC_OID_SECG_EC_SECP128R1       = 211,
    SEC_OID_SECG_EC_SECP128R2       = 212,
    SEC_OID_SECG_EC_SECP160K1       = 213,
    SEC_OID_SECG_EC_SECP160R1       = 214, 
    SEC_OID_SECG_EC_SECP160R2       = 215,
    SEC_OID_SECG_EC_SECP192K1       = 216,
    /* SEC_OID_SECG_EC_SECP192R1 is SEC_OID_ANSIX962_EC_PRIME192V1 */
    SEC_OID_SECG_EC_SECP224K1       = 217,
    SEC_OID_SECG_EC_SECP224R1       = 218,
    SEC_OID_SECG_EC_SECP256K1       = 219,
    /* SEC_OID_SECG_EC_SECP256R1 is SEC_OID_ANSIX962_EC_PRIME256V1 */
    SEC_OID_SECG_EC_SECP384R1       = 220,
    SEC_OID_SECG_EC_SECP521R1       = 221,

    /* ANSI X9.62 named elliptic curves (characteristic two field) */
    SEC_OID_ANSIX962_EC_C2PNB163V1  = 222,
    SEC_OID_ANSIX962_EC_C2PNB163V2  = 223,
    SEC_OID_ANSIX962_EC_C2PNB163V3  = 224,
    SEC_OID_ANSIX962_EC_C2PNB176V1  = 225,
    SEC_OID_ANSIX962_EC_C2TNB191V1  = 226,
    SEC_OID_ANSIX962_EC_C2TNB191V2  = 227,
    SEC_OID_ANSIX962_EC_C2TNB191V3  = 228,
    SEC_OID_ANSIX962_EC_C2ONB191V4  = 229,
    SEC_OID_ANSIX962_EC_C2ONB191V5  = 230,
    SEC_OID_ANSIX962_EC_C2PNB208W1  = 231,
    SEC_OID_ANSIX962_EC_C2TNB239V1  = 232,
    SEC_OID_ANSIX962_EC_C2TNB239V2  = 233,
    SEC_OID_ANSIX962_EC_C2TNB239V3  = 234,
    SEC_OID_ANSIX962_EC_C2ONB239V4  = 235,
    SEC_OID_ANSIX962_EC_C2ONB239V5  = 236,
    SEC_OID_ANSIX962_EC_C2PNB272W1  = 237,
    SEC_OID_ANSIX962_EC_C2PNB304W1  = 238,
    SEC_OID_ANSIX962_EC_C2TNB359V1  = 239,
    SEC_OID_ANSIX962_EC_C2PNB368W1  = 240,
    SEC_OID_ANSIX962_EC_C2TNB431R1  = 241,

    /* SECG named elliptic curves (characteristic two field) */
    SEC_OID_SECG_EC_SECT113R1       = 242,
    SEC_OID_SECG_EC_SECT113R2       = 243,
    SEC_OID_SECG_EC_SECT131R1       = 244,
    SEC_OID_SECG_EC_SECT131R2       = 245,
    SEC_OID_SECG_EC_SECT163K1       = 246,
    SEC_OID_SECG_EC_SECT163R1       = 247,
    SEC_OID_SECG_EC_SECT163R2       = 248,
    SEC_OID_SECG_EC_SECT193R1       = 249,
    SEC_OID_SECG_EC_SECT193R2       = 250,
    SEC_OID_SECG_EC_SECT233K1       = 251,
    SEC_OID_SECG_EC_SECT233R1       = 252,
    SEC_OID_SECG_EC_SECT239K1       = 253,
    SEC_OID_SECG_EC_SECT283K1       = 254,
    SEC_OID_SECG_EC_SECT283R1       = 255,
    SEC_OID_SECG_EC_SECT409K1       = 256,
    SEC_OID_SECG_EC_SECT409R1       = 257,
    SEC_OID_SECG_EC_SECT571K1       = 258,
    SEC_OID_SECG_EC_SECT571R1       = 259,

    SEC_OID_NETSCAPE_AOLSCREENNAME  = 260,

    SEC_OID_AVA_SURNAME              = 261,
    SEC_OID_AVA_SERIAL_NUMBER        = 262,
    SEC_OID_AVA_STREET_ADDRESS       = 263,
    SEC_OID_AVA_TITLE                = 264,
    SEC_OID_AVA_POSTAL_ADDRESS       = 265,
    SEC_OID_AVA_POSTAL_CODE          = 266,
    SEC_OID_AVA_POST_OFFICE_BOX      = 267,
    SEC_OID_AVA_GIVEN_NAME           = 268,
    SEC_OID_AVA_INITIALS             = 269,
    SEC_OID_AVA_GENERATION_QUALIFIER = 270,
    SEC_OID_AVA_HOUSE_IDENTIFIER     = 271,
    SEC_OID_AVA_PSEUDONYM            = 272,

    /* More OIDs */
    SEC_OID_PKIX_CA_ISSUERS          = 273,
    SEC_OID_PKCS9_EXTENSION_REQUEST  = 274,

    /* new EC Signature oids */
    SEC_OID_ANSIX962_ECDSA_SIGNATURE_RECOMMENDED_DIGEST = 275,
    SEC_OID_ANSIX962_ECDSA_SIGNATURE_SPECIFIED_DIGEST = 276,
    SEC_OID_ANSIX962_ECDSA_SHA224_SIGNATURE = 277,
    SEC_OID_ANSIX962_ECDSA_SHA256_SIGNATURE = 278,
    SEC_OID_ANSIX962_ECDSA_SHA384_SIGNATURE = 279,
    SEC_OID_ANSIX962_ECDSA_SHA512_SIGNATURE = 280,

    /* More id-ce and id-pe OIDs from RFC 3280 */
    SEC_OID_X509_HOLD_INSTRUCTION_CODE      = 281,
    SEC_OID_X509_DELTA_CRL_INDICATOR        = 282,
    SEC_OID_X509_ISSUING_DISTRIBUTION_POINT = 283,
    SEC_OID_X509_CERT_ISSUER                = 284,
    SEC_OID_X509_FRESHEST_CRL               = 285,
    SEC_OID_X509_INHIBIT_ANY_POLICY         = 286,
    SEC_OID_X509_SUBJECT_INFO_ACCESS        = 287,

    /* Camellia OIDs (RFC3657)*/
    SEC_OID_CAMELLIA_128_CBC                = 288,
    SEC_OID_CAMELLIA_192_CBC                = 289,
    SEC_OID_CAMELLIA_256_CBC                = 290,

    /* PKCS 5 V2 OIDS */
    SEC_OID_PKCS5_PBKDF2                    = 291,
    SEC_OID_PKCS5_PBES2                     = 292,
    SEC_OID_PKCS5_PBMAC1                    = 293,
    SEC_OID_HMAC_SHA1                       = 294,
    SEC_OID_HMAC_SHA224                     = 295,
    SEC_OID_HMAC_SHA256                     = 296,
    SEC_OID_HMAC_SHA384                     = 297,
    SEC_OID_HMAC_SHA512                     = 298,

    SEC_OID_PKIX_TIMESTAMPING               = 299,
    SEC_OID_PKIX_CA_REPOSITORY              = 300,

    SEC_OID_ISO_SHA1_WITH_RSA_SIGNATURE     = 301,

    SEC_OID_TOTAL
} SECOidTag;

#define SEC_OID_SECG_EC_SECP192R1 SEC_OID_ANSIX962_EC_PRIME192V1
#define SEC_OID_SECG_EC_SECP256R1 SEC_OID_ANSIX962_EC_PRIME256V1
#define SEC_OID_PKCS12_KEY_USAGE  SEC_OID_X509_KEY_USAGE

/* fake OID for DSS sign/verify */
#define SEC_OID_SHA SEC_OID_MISS_DSS

typedef enum {
    INVALID_CERT_EXTENSION = 0,
    UNSUPPORTED_CERT_EXTENSION = 1,
    SUPPORTED_CERT_EXTENSION = 2
} SECSupportExtenTag;

struct SECOidDataStr {
    SECItem            oid;
    SECOidTag          offset;
    const char *       desc;
    unsigned long      mechanism;
    SECSupportExtenTag supportedExtension;	
    				/* only used for x.509 v3 extensions, so
				   that we can print the names of those
				   extensions that we don't even support */
};

#endif /* _SECOIDT_H_ */
