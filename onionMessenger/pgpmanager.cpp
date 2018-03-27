#include "pgpmanager.h"
#include "common.h"

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
        // maybe regex better?
        int index = 0;
        while(dec.find('"',index) != std::string::npos) {
            index = dec.find('"',index) + 1;
        }
        dec = dec.substr(index + 1); // line feed is 1 byte
        return dec;
    }

    void PGPManager::ImportKeys() {
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
        command = "echo \"";
        // how to do this securely?
        command.append(this->passPhrase);
        command.append("\" | gpg --import ");
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
        if(output.find("failed") != std::string::npos || output.find("error") != std::string::npos) {
            cout << "Please check your passphrase" << endl;
            exit(1);
        }
        fclose(pipe);
        // for debugging purpose only
        output = "";
        command = "gpg --list-secret-keys 2>&1";
        pipe = popen(command.c_str(), "r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return;
        }
        while( (c = fgetc(pipe)) != EOF ) {
            output.push_back(c);
        }
        cout << "Debug: " << output << endl;
        fclose(pipe);
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
