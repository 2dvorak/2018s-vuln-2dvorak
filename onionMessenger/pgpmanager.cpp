#include "pgpmanager.h"

namespace PGPCrypt{

    PGPManager::PGPManager(){}

    PGPManager::~PGPManager(){}

    string PGPManager::Enc(string input_plain, string recipientID){
        FILE *pipe;
        string enc = "";
        int c;
        string command = "echo \"";
        command.append(input_plain);
        command.append("\" | gpg --encrypt --armor --yes --trust-model always -r ");
        command.append(recipientID);
        command.append(" 2>&1");
        pipe = popen(command.c_str(), "r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return "";
        }
        while( (c = fgetc(pipe)) != EOF ) {
            enc.push_back(c);
        }
        fclose(pipe);
        return enc;
    }

    string PGPManager::Dec(string input_cipher){
        FILE *pipe;
        string dec = "";
        int c;
        string command = "echo \"";
        command.append(input_cipher);
        command.append("\" | gpg --decrypt 2>&1");
        pipe = popen(command.c_str(),"r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return "";
        }
        while( (c = fgetc(pipe)) != EOF ) {
            dec.push_back(c);
        }
        fclose(pipe);
        return dec;
    }

    void PGPManager::ImportKeys() {
        FILE *pipe;
        int c;
        string output = "";
        string githubID = "";
        cout << "Your Github ID :";
        cin >> githubID;
        // check if valid githubID
        // NO COMMAND INJECTION ALLOWED EVER
        // "Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen" - Github
        if( CheckID(githubID) ) {
            cout << "Invalid Github ID!" << endl;
            exit(1);
        }
        // how to input passphrase securely?
        // stdinNoEcho();
        cout << "Your passphrase :";
        cin >> this->passPhrase;
        string command = "gpg --import ";
        command.append(githubID);
        command.append(".pub 2>&1");
        pipe = popen(command.c_str(), "r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return;
        }
        while( (c = fgetc(pipe)) != EOF ) {
            output.push_back(c);
        }
        fclose(pipe);
        command = "echo \"";
        // how to do this securely?
        command.append(this->passPhrase);
        command.append("\" | gpg --import ");
        command.append(githubID);
        command.append(".key 2>&1");
        pipe = popen(command.c_str(), "r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return;
        }
        while( (c = fgetc(pipe)) != EOF ) {
            output.push_back(c);
        }
        fclose(pipe);
    }
}
