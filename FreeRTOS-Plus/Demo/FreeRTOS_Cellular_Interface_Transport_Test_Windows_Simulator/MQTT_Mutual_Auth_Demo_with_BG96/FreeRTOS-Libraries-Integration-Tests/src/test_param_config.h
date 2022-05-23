/*
 * FreeRTOS FreeRTOS LTS Qualification Tests preview
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file test_param_config.h
 * @brief This setup the test parameters for LTS qualification test.
 */

#ifndef TEST_PARAM_CONFIG_H
#define TEST_PARAM_CONFIG_H

/**
 * @brief Configuration that indicates if the device should generate a key pair.
 *
 * @note When FORCE_GENERATE_NEW_KEY_PAIR is set to 1, the device should generate
 * a new on-device key pair and output public key. When set to 0, the device
 * should keep existing key pair.
 *
 * #define FORCE_GENERATE_NEW_KEY_PAIR   0
 */

/**
 * @brief Endpoint of the MQTT broker to connect to in mqtt test.
 *
 * #define MQTT_SERVER_ENDPOINT   "PLACE_HOLDER"
 */

/**
 * @brief Port of the MQTT broker to connect to in mqtt test.
 *
 * #define MQTT_SERVER_PORT       (8883)
 */

/**
 * @brief Root certificate of the IoT Core.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define IOT_CORE_ROOT_CA NULL
 */

/**
 * @brief Client certificate to connect to MQTT server.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define MQTT_CLIENT_CERTIFICATE NULL
 */

/**
 * @brief Client private key to connect to MQTT server.
 *
 * @note This is should only be used for testing purpose.
 *
 * For qualification, the key should be generated on-device.
 *
 * #define MQTT_CLIENT_PRIVATE_KEY  NULL
 */

/**
 * @brief Endpoint of the echo server to connect to in transport interface test.
 *
 * #define ECHO_SERVER_ENDPOINT   "PLACE_HOLDER"
 */
#define ECHO_SERVER_ENDPOINT "ec2-52-195-10-90.ap-northeast-1.compute.amazonaws.com"


/**
 * @brief Port of the echo server to connect to in transport interface test.
 *
 * #define ECHO_SERVER_PORT       (9000)
 */
#define ECHO_SERVER_PORT       (9000)

/**
 * @brief Root certificate of the echo server.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define ECHO_SERVER_ROOT_CA "PLACE_HOLDER"
 */
#define ECHO_SERVER_ROOT_CA \
"-----BEGIN CERTIFICATE-----\n"\
"MIIEPTCCAyWgAwIBAgIUEfmVsyRfhtohAl7qpWpnoG+djv0wDQYJKoZIhvcNAQEL\n"\
"BQAwga0xCzAJBgNVBAYTAlVTMQswCQYDVQQIDAJXQTEOMAwGA1UEBwwFUGxhY2Ux\n"\
"FDASBgNVBAoMC1lvdXJDb21wYW55MQswCQYDVQQLDAJJVDE+MDwGA1UEAww1ZWMy\n"\
"LTUyLTE5NS0xMC05MC5hcC1ub3J0aGVhc3QtMS5jb21wdXRlLmFtYXpvbmF3cy5j\n"\
"b20xHjAcBgkqhkiG9w0BCQEWD291c2NAYW1hem9uLmNvbTAeFw0yMjA1MjAwOTA2\n"\
"NTdaFw0yMzA1MjAwOTA2NTdaMIGtMQswCQYDVQQGEwJVUzELMAkGA1UECAwCV0Ex\n"\
"DjAMBgNVBAcMBVBsYWNlMRQwEgYDVQQKDAtZb3VyQ29tcGFueTELMAkGA1UECwwC\n"\
"SVQxPjA8BgNVBAMMNWVjMi01Mi0xOTUtMTAtOTAuYXAtbm9ydGhlYXN0LTEuY29t\n"\
"cHV0ZS5hbWF6b25hd3MuY29tMR4wHAYJKoZIhvcNAQkBFg9vdXNjQGFtYXpvbi5j\n"\
"b20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCn2y3vyajLKCFbI2ax\n"\
"54az7HCZVaVXHLYw4XBQepXtdTp1S0+ZkQRhK65AF+QbHpkopkyIUPkecDcae/ap\n"\
"cexgn7JHioWUIy6QG7QQ+pc3afNb7CXNbivCdXvqpxsoZuN8/eLnCMVVa5QZQ+4D\n"\
"r3q+hqOusFlcpOMIqYa8O8YBv++wIO0bjYsc4sRMX0UKRt2L6kht18+eVRxUaXPb\n"\
"YGBCKDsgIuWA3gY15s8gG2RChMUv2BRsFffJYtpbNf3P5GdhD6wl0C6Fto9CUnUi\n"\
"lWLm6AnHhXabEmUrNuWM4icKEi27Lpj/46hzVrBkkfNqxMdWRlnt3w8J6HOQwvok\n"\
"sJktAgMBAAGjUzBRMB0GA1UdDgQWBBQCcRp/tRLz4/Wl6QrYPjskp7OFcjAfBgNV\n"\
"HSMEGDAWgBQCcRp/tRLz4/Wl6QrYPjskp7OFcjAPBgNVHRMBAf8EBTADAQH/MA0G\n"\
"CSqGSIb3DQEBCwUAA4IBAQBP0zPrP47icP4eoq3v2qPXf1buPqfZGMVBHIeoCb2G\n"\
"q5oI1RuayNihtDiwkgLc+kOn8URtIJs4LmMxfq6i3w+7zAGxF5XcfZReXACof200\n"\
"H8vKItIFRK0+04Capg8mHHfh/rmd3iP23DRV0dd1X2T8Z8QsYKvC8zEEPZZbQ1do\n"\
"muRMtMYI19pge2Ae8zDis9Y64zttln5TuQSW9pGungMWJthd/uysy5ecDLkLBL5q\n"\
"OP5oButq3QLRmI1LHccDnynoeGm/m7guhKRwbtYf96S4q7K5xxfa6HZ5buP1P1if\n"\
"/DTa2ZT4P4jDF/NY+cD8SMeR+BQgcGa5xMbZ6h467fYE\n"\
"-----END CERTIFICATE-----\n"



/**
 * @brief Client certificate to connect to echo server.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define TRANSPORT_CLIENT_CERTIFICATE NULL
 */
#define TRANSPORT_CLIENT_CERTIFICATE \
"-----BEGIN CERTIFICATE-----\n"\
"MIID4zCCAssCFG+NepOQcTpdcgShHQUkSXfqbDOJMA0GCSqGSIb3DQEBCwUAMIGt\n"\
"MQswCQYDVQQGEwJVUzELMAkGA1UECAwCV0ExDjAMBgNVBAcMBVBsYWNlMRQwEgYD\n"\
"VQQKDAtZb3VyQ29tcGFueTELMAkGA1UECwwCSVQxPjA8BgNVBAMMNWVjMi01Mi0x\n"\
"OTUtMTAtOTAuYXAtbm9ydGhlYXN0LTEuY29tcHV0ZS5hbWF6b25hd3MuY29tMR4w\n"\
"HAYJKoZIhvcNAQkBFg9vdXNjQGFtYXpvbi5jb20wHhcNMjIwNTIwMDkwNzM3WhcN\n"\
"MjMwNTIwMDkwNzM3WjCBrTELMAkGA1UEBhMCVVMxCzAJBgNVBAgMAldBMQ4wDAYD\n"\
"VQQHDAVQbGFjZTEUMBIGA1UECgwLWW91ckNvbXBhbnkxCzAJBgNVBAsMAklUMT4w\n"\
"PAYDVQQDDDVlYzItNTItMTk1LTEwLTkwLmFwLW5vcnRoZWFzdC0xLmNvbXB1dGUu\n"\
"YW1hem9uYXdzLmNvbTEeMBwGCSqGSIb3DQEJARYPb3VzY0BhbWF6b24uY29tMIIB\n"\
"IjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAoao3JGoGzKhba8yTYg6cHtoj\n"\
"1ucFGiXcm0uWaOkIvJ9abbgqPh8t5g46+ctmW45RGOS8AXq98TRWR2UIYTzQS7gz\n"\
"CAaC7f+Ow6w3JMW34ZbdMAHuVsavtv9lVWGzUlCFo5lwvo8fk6pUaidqGD1XCnjT\n"\
"yqz6SIYWUyWU7uYK8AmKeSp1RUaidv5ndTYh8n04qT5cia91Nvddg7n559d1tl/8\n"\
"/bSjcVRwsCiK3D6iLUfprmy8Duh2pw3INEMuuqddOaGGNMhP3ULipWbeMhBAndN8\n"\
"ozKXEBOhOmafABMNtBHBktfINLu7Gqx/MXSFqCwZO6G+3jaFnq9ebDtUSGJDYQID\n"\
"AQABMA0GCSqGSIb3DQEBCwUAA4IBAQBiCfqMXgN1fVkPm/CkvVFwF7nSWN9eKCNF\n"\
"RwZy2l43+aka22CysmwTQMCi80QExLmSr3h+8kbwboVKhCqBpD2dSxXEgqFaHxWJ\n"\
"gCdbLYeaKs8W6IKAxH9pu2yQS1BewiWSk6ZbZV9ZPh3Ihc5k7mzj4TPJdxQbLwC2\n"\
"DzeoVtqOq2vgwwj4+R4kMAtvge+64BLAPyZ+ds7MzfFkuQ29LlFqeF5sUgr9GAe+\n"\
"7MkuDJ6jbSyHXC96v/PY0uaiCSaCuD4kW/07BIb8OmaMmzQXd25duIPJ+D/IZJg6\n"\
"/JIO6iCfwfUdySsN4eixTQEUCakqQIsCmcLsO6eDYNsDkpq7xJ2L\n"\
"-----END CERTIFICATE-----\n"




/**
 * @brief Client private key to connect to echo server.
 *
 * @note This is should only be used for testing purpose.
 *
 * For qualification, the key should be generated on-device.
 *
 * #define TRANSPORT_CLIENT_PRIVATE_KEY  NULL
 */
#define TRANSPORT_CLIENT_PRIVATE_KEY  \
"-----BEGIN PRIVATE KEY-----\n"\
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQChqjckagbMqFtr\n"\
"zJNiDpwe2iPW5wUaJdybS5Zo6Qi8n1ptuCo+Hy3mDjr5y2ZbjlEY5LwBer3xNFZH\n"\
"ZQhhPNBLuDMIBoLt/47DrDckxbfhlt0wAe5Wxq+2/2VVYbNSUIWjmXC+jx+TqlRq\n"\
"J2oYPVcKeNPKrPpIhhZTJZTu5grwCYp5KnVFRqJ2/md1NiHyfTipPlyJr3U2912D\n"\
"ufnn13W2X/z9tKNxVHCwKIrcPqItR+mubLwO6HanDcg0Qy66p105oYY0yE/dQuKl\n"\
"Zt4yEECd03yjMpcQE6E6Zp8AEw20EcGS18g0u7sarH8xdIWoLBk7ob7eNoWer15s\n"\
"O1RIYkNhAgMBAAECggEAA/3qPSeyEckhcsS7Ketu7vMXefW74kSnkR0Zcm84pEGy\n"\
"D64BuwV1km3tBmB/n0ihF4KEYuv3NzbB36LuxUnlYc1DU1In1v0dtxawSOPp8bPS\n"\
"M96EVlezHItrYuOV9DUhS6QdDNOHZdPJoFJnVABlvxAzOJizWGmZMQXnSsva3sjv\n"\
"erAXUqQCJqo0yx7lmkOYBWBke7ouSO1YxbQ8wCdoMzcfnRntmO6X0QbKvcje6OQ3\n"\
"IcpEKqwGDjgv59agdvXyq9oWI6zc75qa5ZgxGLQQ42DpfrPMhYSkOqi3f5cJEIs7\n"\
"eGdIcy8r7MgTDtpUIvGzu56emnDSXG9/zIC5tuUF7QKBgQDLSqbVT+Gk/VPRsn7E\n"\
"K32FTF+17W9CwaWLOEO/+odNtXNGJ01d5JNihS6xNoY1Lcn6s8m/cU/v3HGDB4W7\n"\
"des/uqRcZpOemiKxt7Q9065HVmaUKHxmY6/wtKgdfV08I8O+uGGlejnKg4HqphW2\n"\
"xWcjHKsm+PSoZ+CXcLjyszG+nQKBgQDLlJ5Hc3apciy1HFfwZjENV1i4i+4/94iG\n"\
"jPQJ2k4BEXHIo3UzdWPg3Y/G99iCO90BXlDj3iYnmSzrl6zU0l+Mx1y2/kHiWP1j\n"\
"UmAOxCfmx2XPMILR+5R+SY8uNi2CN0/yYSobOCy79VUUxf/umbVkBBfl1j2Vq0MD\n"\
"ki/O6p36lQKBgAbcii0A+Ti8ZCvbvnOYlC//l+toDC1Qo+nDCGZealqxGSbxBKd6\n"\
"o47bt+u7T2RUG1Ee6FxR0sThx+MISfc9z6bjAdEslSbxROKCtM6xalx2ChhOFYTP\n"\
"MI0LS/FS3ea9vkmdp44SRgkTzzrASFNW7lg9C3nRRVC50NbLszJd4fvxAoGAQP9G\n"\
"OZHHxPnLBFf0S7CU4KmOjf0/J/UkBbFjCPJGDVlAdsqZVMCaDuUuTlw7J/ls6k7u\n"\
"jJIUpHfnO6DFcX1pPE06cMJkzpuS6u8V9CD0jF4FR005t0kXc+qQaDSr+vMl90DG\n"\
"bMJWpfwCGgvNVqSEoVYdOyhXqT1wu+cklG+b2LECgYAqQPHk1YrXbtjV/IGn+4hi\n"\
"DTCxDJ+kbIf59Rvr5QuBaMfZwm7Ytr6iamiZc9KInVHjuZRKC0r1qw7/LMazB76o\n"\
"nDOgcYDd8EAlnf10Ac9GrYXA2xAvyycv9t38dyhHiMlM6DfKlQFzQjGj0wXkVkc8\n"\
"/JPA691+3Jd3QwWDcpmjSA==\n"\
"-----END PRIVATE KEY-----\n"\





/**
 * @brief Microseconds to delay the start of test.
 *
 * @note This configuration is optional. Default is 5000.
 *
 * #define TEST_START_DELAY_MS  5000
 */

#define OTA_RSA_SHA1      1
#define OTA_RSA_SHA256    2
#define OTA_ECDSA_SHA256  3
/**
 * @brief Certificate type for OTA PAL test.
 * Valid options are: OTA_RSA_SHA1, OTA_RSA_SHA256, OTA_ECDSA_SHA256.
 *
 * #define OTA_PAL_TEST_CERT_TYPE OTA_ECDSA_SHA256
 */

/**
 * @brief Path to cert for OTA test PAL. Used to verify signature.
 * If applicable, the device must be pre-provisioned with this certificate. Please see
 * test/common/ota/test_files for the set of certificates.
 *
 * #define OTA_PAL_CERTIFICATE_FILE    "ecdsa-sha256-signer.crt.pem"
 */

/**
 * @brief Some devices have a hard-coded name for the firmware image to boot.
 *
 * #define OTA_PAL_FIRMWARE_FILE   "dummy.bin"
 */

/**
 * @brief Some boards OTA PAL layers will use the file names passed into it for the
 * image and the certificates because their non-volatile memory is abstracted by a
 * file system. Set this to 1 if that is the case for your device.
 *
 * #define OTA_PAL_USE_FILE_SYSTEM                          0
 */

/**
 * @brief The PKCS #11 supports RSA key function.
 *
 * Set to 1 if RSA private keys are supported by the platform. 0 if not.
 *
 * #define PKCS11_TEST_RSA_KEY_SUPPORT                     ( 1 )
 */

/**
 * @brief The PKCS #11 supports EC key function.
 *
 * Set to 1 if elliptic curve private keys are supported by the platform. 0 if not.
 *
 * #define PKCS11_TEST_EC_KEY_SUPPORT                      ( 1 )
 */

/**
 * @brief The PKCS #11 supports import key method.
 *
 * Set to 1 if importing device private key via C_CreateObject is supported. 0 if not.
 *
 * #define PKCS11_TEST_IMPORT_PRIVATE_KEY_SUPPORT          ( 1 )
 */

/**
 * @brief The PKCS #11 supports generate keypair method.
 *
 * Set to 1 if generating a device private-public key pair via C_GenerateKeyPair. 0 if not.
 *
 * #define PKCS11_TEST_GENERATE_KEYPAIR_SUPPORT            ( 1 )
 */

/**
 * @brief The PKCS #11 supports preprovisioning method.
 *
 * Set to 1 if preprovisioning is supported.
 *
 * #define PKCS11_TEST_PREPROVISIONED_SUPPORT              ( 0 )
 */

/**
 * @brief The PKCS #11 label for device private key for test.
 *
 * For devices with on-chip storage, this should match the non-test label.
 * For devices with secure elements or hardware limitations, this may be defined
 * to a different label to preserve AWS IoT credentials for other test suites.
 *
 * #define PKCS11_TEST_LABEL_DEVICE_PRIVATE_KEY_FOR_TLS    pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS
 */

/**
 * @brief The PKCS #11 label for device public key.
 *
 * For devices with on-chip storage, this should match the non-test label.
 * For devices with secure elements or hardware limitations, this may be defined
 * to a different label to preserve AWS IoT credentials for other test suites.
 *
 * #define PKCS11_TEST_LABEL_DEVICE_PUBLIC_KEY_FOR_TLS     pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS
 */

/**
 * @brief The PKCS #11 label for the device certificate.
 *
 * For devices with on-chip storage, this should match the non-test label.
 * For devices with secure elements or hardware limitations, this may be defined
 * to a different label to preserve AWS IoT credentials for other test suites.
 *
 * #define PKCS11_TEST_LABEL_DEVICE_CERTIFICATE_FOR_TLS    pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS
 */

/**
 * @brief The PKCS #11 supports storage for JITP.
 *
 * The PKCS11 test will verify the following labels with create/destroy objects.
 * PKCS11_TEST_LABEL_CODE_VERIFICATION_KEY
 * PKCS11_TEST_LABEL_JITP_CERTIFICATE
 * PKCS11_TEST_LABEL_ROOT_CERTIFICATE
 * Set to 1 if PKCS #11 supports storage for JITP certificate, code verify certificate,
 * and trusted server root certificate.
 *
 * #define PKCS11_TEST_JITP_CODEVERIFY_ROOT_CERT_SUPPORTED pkcs11configJITP_CODEVERIFY_ROOT_CERT_SUPPORTED
 */

/**
 * @brief The PKCS #11 label for the object to be used for code verification.
 *
 * This label has to be defined if PKCS11_TEST_JITP_CODEVERIFY_ROOT_CERT_SUPPORTED is set to 1.
 *
 * #define PKCS11_TEST_LABEL_CODE_VERIFICATION_KEY    pkcs11configLABEL_CODE_VERIFICATION_KEY
 */

/**
 * @brief The PKCS #11 label for Just-In-Time-Provisioning.
 *
 * This label has to be defined if PKCS11_TEST_JITP_CODEVERIFY_ROOT_CERT_SUPPORTED is set to 1.
 *
 * #define PKCS11_TEST_LABEL_JITP_CERTIFICATE    pkcs11configLABEL_JITP_CERTIFICATE
 */

/**
 * @brief The PKCS #11 label for the AWS Trusted Root Certificate.
 *
 * This label has to be defined if PKCS11_TEST_JITP_CODEVERIFY_ROOT_CERT_SUPPORTED is set to 1.
 *
 * #define PKCS11_TEST_LABEL_ROOT_CERTIFICATE    pkcs11configLABEL_ROOT_CERTIFICATE
 */

/**
 * @brief The IoT Thing name for the device for OTA test.
 *
 * #define OTA_IOT_THING_NAME  "PLACE_HOLDER"
 */

/**
 * @brief Major version for OTA E2E test.
 *
 * #define OTA_APP_VERSION_MAJOR                            0
 */

/**
 * @brief Major version for OTA E2E test.
 *
 * #define OTA_APP_VERSION_MINOR                            9
 */

/**
 * @brief Major version for OTA E2E test.
 *
 * #define OTA_APP_VERSION_BUILD                            1
 */


#endif /* TEST_PARAM_CONFIG_H */
