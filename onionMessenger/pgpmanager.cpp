#include "pgpmanager.h"
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

namespace PGPCrypt{

#define fail_if_err(err)\
  do {\
      if (err)\
        {\
          fprintf (stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, gpgme_strsource (err),gpgme_strerror (err));     \
          exit(1);\
        }\
    } while (0)

    PGPManager::PGPManager(string pass){
        /* Initializes gpgme */
        gpgme_error_t error;

        gpgme_check_version (NULL);

        /* Initialize the locale environment.  */
        setlocale (LC_ALL, "");
        gpgme_set_locale (NULL, LC_CTYPE, setlocale (LC_CTYPE, NULL));
        #ifdef LC_MESSAGES
          gpgme_set_locale (NULL, LC_MESSAGES, setlocale (LC_MESSAGES, NULL));
        #endif

        error = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
        fail_if_err(error);

        PGPManager::SetPassPhrase(pass);
    }

    PGPManager::~PGPManager(){}

    void PGPManager::SetPubKey(string a_pubkey){
        this->pubKey = a_pubkey;
    }

    void PGPManager::SetPriKey(string a_privkey){
        this->privKey = a_privkey;
    }

    void PGPManager::SetPassPhrase(string a_passphrase){
        this->passPhrase = a_passphrase;
    }

    gpgme_error_t passphrase_cb (void *opaque, const char *uid_hint, const char *passphrase_info,
               int last_was_bad, int fd)
    {
        int res;
        char *pass = (char *)opaque;
        int passlen = strlen(pass);
        int off = 0;

        do{ res = write (fd, &pass[off], passlen - off);
            if (res > 0)
            off += res;
        } while (res > 0 && off != passlen);

        return off == passlen ? 0 : gpgme_error_from_errno (errno);
    }
    bool PGPManager::KeyValidation(string passphrase){}

    // https://github.com/seiyak/GPGME-sample-code // t-decrypt.c, t-encrypt.c
    // http://www.bortzmeyer.org/files/gpgme-encrypt.c
    string PGPManager::Enc(string input_plain){
        gpgme_ctx_t context;
        gpgme_error_t error;
        gpgme_data_t plain, cipher, pgpkey;
        gpgme_key_t keys[3] = { NULL, NULL, NULL};
        gpgme_key_t key;
        gpgme_import_result_t result;
        char *buffer;
        ssize_t nbytes;

        //  gpgme_new -> getkey from memory -> get plain text data -> encrypt
        error = gpgme_new (&context);
        fail_if_err(error);

        // set armor mode
        gpgme_set_armor(context, 1);

        /* Initialize input buffer. */
        // memory value to gpgme_data_t ( public key )
        // gpgme_data_new_from_mem(&gpgme_data_t, char *, len(char *), 0)
        error = gpgme_data_new_from_mem(&pgpkey, this->pubKey.c_str(), this->pubKey.size(), 0);
        fail_if_err(error);

        // https://www.gnupg.org/documentation/manuals/gpgme/Importing-Keys.html
        // gpgme_op_import key from pgpkey
        error = gpgme_op_import(context, pgpkey);
        fail_if_err(error);
        
        /* Search key for encryption. */
        // get key
        result = gpgme_op_import_result(context);
        error = gpgme_get_key(context, result->imports->fpr, &key, 1);
        fail_if_err(error);

        /* Initialize input buffer. */
        // memory value to gpgme_data_t ( plain text == input value )
        error = gpgme_data_new_from_mem(&plain, input_plain.c_str(), input_plain.size(), 0);
        fail_if_err(error);

        /* Initialize input buffer. */
        // If no initialize, then Bus error
        error = gpgme_data_new(&cipher);
        fail_if_err(error);

        /* Encrypt data. */
        // encrypt
        keys[0] = key;
        error = gpgme_op_encrypt(context, keys, GPGME_ENCRYPT_ALWAYS_TRUST, plain, cipher);
        fail_if_err(error);

        nbytes = gpgme_data_seek(cipher, 0, SEEK_SET);
        buffer = (char *)malloc(4096);
        nbytes = gpgme_data_read(cipher, buffer, 4096);
        buffer[nbytes] = '\0';
        string return_s = string(buffer, nbytes);
        free(buffer);
        return return_s;
    }

    string PGPManager::Dec(string input_cipher){
        gpgme_ctx_t context;
        gpgme_error_t error;
        gpgme_data_t plain, cipher, pgpkey;
        gpgme_import_result_t result;
        gpgme_key_t privKey_gpgme_t;
        char *buffer;
        ssize_t nbytes;

        //  gpgme_new -> getkey from memory -> get cipher text data -> decrypt
        error = gpgme_new (&context);
        fail_if_err(error);

        // set armor mode
        gpgme_set_armor(context, 1);

        // https://www.gnupg.org/documentation/manuals/gpgme/Passphrase-Callback.html#Passphrase-Callback
        // setting passphrase_cb
        // gpgme_set_pinentry_mode(context, GPGME_PINENTRY_MODE_LOOPBACK);
        // this->pp = (char *)malloc(this->passPhrase.size());
        // gpgme_set_passphrase_cb(context, passphrase_cb, *this->pp);
        // free(this->pp);

        /* Initialize input buffer. */
        // memory value to gpgme_data_t ( private key )
        error = gpgme_data_new_from_mem(&pgpkey, this->privKey.c_str(), this->privKey.size(), 0);
        fail_if_err(error);

        // gpgme_op_import key from pgpkey
        error = gpgme_op_import(context, pgpkey);
        fail_if_err(error);

        /* Search key for decryption. */
        // get key
        result = gpgme_op_import_result(context);
        error = gpgme_get_key(context, result->imports->fpr, &privKey_gpgme_t, 1);
        fail_if_err(error);

        /* Initialize input buffer. */
        // memory value to gpgme_data_t ( plain text == input value )
        error = gpgme_data_new_from_mem(&cipher, input_cipher.c_str(), input_cipher.size(), 1);
        fail_if_err(error);

        /* Initialize input buffer. */
        // If no initialize, then Bus error
        error = gpgme_data_new(&plain);
        fail_if_err(error);

        /* Decrypt data. */
        // decrypt
        error = gpgme_op_decrypt(context, cipher, plain);
        fail_if_err (error);

        nbytes = gpgme_data_seek(plain, 0, SEEK_SET);
        buffer = (char *)malloc(4096);
        nbytes = gpgme_data_read(plain, buffer, 4096);
        buffer[nbytes] = '\0';
        string return_s = string(buffer, nbytes);
        free(buffer);
        return return_s;
    }
}
