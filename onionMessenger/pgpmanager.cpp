#include "pgpmanager.h"
#include "common.h"
#include <fstream>

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
        string randomFile = "";
        srand(time(NULL));
        for(int i = 0 ; i < 10 ; i++) {
            randomFile.push_back('a' + rand()%26);
        }
        ofstream tmpFile(randomFile.data());
        if(tmpFile.is_open()) {
            tmpFile << input_cipher << EOF;
            tmpFile.close();
        }
        int c;
        // gpg --passphrase "myPassphrase" --decrypt file
        string command = "gpg --passphrase \"";
        command.append(this->passPhrase);
        command.append("\" --decrypt ");
        command.append(randomFile);
        command.append(" 2>&1");
        command.append(";rm "+randomFile);
        pipe = popen(command.c_str(),"r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return "";
        }
        while( (c = fgetc(pipe)) != EOF ) {
            dec.push_back(c);
        }
        fclose(pipe);
        // maybe regex better?
        int index = 0;
        while(dec.find('"',index) != std::string::npos) {
            index = dec.find('"',index) + 1;
        }
        dec = dec.substr(index + 1); // line feed is 1 byte
        dec.erase(dec.find("\n"),1);
        return dec;
    }

    void PGPManager::ImportPub(string pub){
        FILE *pipe;
        string randomFile = "";
        int c;
        string output = "";
        srand(time(NULL));
        for(int i = 0 ; i < 10 ; i++) {
            randomFile.push_back('a' + rand()%26);
        }
        ofstream tmpFile(randomFile.data());
        if(tmpFile.is_open()) {
            tmpFile << pub << EOF;
            tmpFile.close();
        }

        string command = "gpg --import " + randomFile + " 2>&1;rm " + randomFile;

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

    void PGPManager::ImportKeys(string githubID) {
        FILE *pipe;
        int c;
        string output = "";

        // how to input passphrase securely?
        SetTTYEcho(false);
        cout << "Your passphrase :";
        cin >> this->passPhrase;
        SetTTYEcho(true);
        cout << endl;
        string command = "gpg --import ";
        command.append(g_km->ReturnGithubID());
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
        output = "";
        // how to do this securely?
        // nope. this anyway prompts passphrase input...
        //command = "echo \"";
        //command.append(this->passPhrase);
        command = "gpg --batch --import ";       // why --batch do not ask for passphrase?
        command.append(g_km->ReturnGithubID());
        command.append(".key 2>&1");
        pipe = popen(command.c_str(), "r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return;
        }
        while( (c = fgetc(pipe)) != EOF ) {
            output.push_back(c);
        }
        // secret key import don't care about passphrase.why?
        /*if(output.find("failed") != std::string::npos || output.find("error") != std::string::npos || output.find("not") != std::string::npos) {
            //cout << output << endl;
            cout << "Please check your passphrase" << endl;
            exit(1);
        }*/
        //for debugging
        //cout << output << endl;
        fclose(pipe);
        // for debugging purpose only
        //        output = "";
        //        command = "gpg --list-secret-keys 2>&1";
        //        pipe = popen(command.c_str(), "r");
        //        if( pipe == NULL) {
        //            perror("popen failed\n");
        //            return;
        //        }
        //        while( (c = fgetc(pipe)) != EOF ) {
        //            output.push_back(c);
        //        }
        //        cout << "Debug: " << output << endl;
        //        fclose(pipe);
        string testing = "JustForTestString!@#";
        string test_enc = Enc(testing, githubID);
        string test_dec = Dec(test_enc);
        if(testing.compare(test_dec) != 0){
            cout << "Check your PW plz!" << endl;
            exit(1);
        }

    }

    void PGPManager::SetTTYEcho(bool enable) {
        struct termios tty;
        tcgetattr(STDIN_FILENO, &tty);
        if( !enable ) {
            tty.c_lflag &= ~ECHO;
        } else {
            tty.c_lflag |= ECHO;
        }
        (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    }
}
