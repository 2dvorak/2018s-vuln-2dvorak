#include "pgpmanager.h"
#include "common.h"
#include <fstream>
#include <regex>

namespace PGPCrypt{

    PGPManager::PGPManager(){}

    PGPManager::~PGPManager(){}

    std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to){
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    string PGPManager::Enc(string input_plain, string recipientFpr){
        FILE *pipe;
        string enc = "";
        string change_plain = "";
        int c;

        change_plain = ReplaceAll(input_plain, std::string("\""), std::string("\\\""));
        string command = "echo \"";
        command.append(change_plain);
        command.append("\" | gpg --encrypt --armor --yes --trust-model always -r ");
        command.append(recipientFpr);
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
        struct timeval tp;
        gettimeofday(&tp, NULL);
        unsigned long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        std::regex e("gpg:\\sencrypted\\swith\\s\\d+-bit\\sRSA\\skey,\\sID\\s[\\w\\d]+,\\screated\\s\\d\\d\\d\\d-\\d\\d-\\d\\d\\n\\s+\".*?\"\\n");
        std::smatch m;

        srand(ms);
        for(int i = 0 ; i < 10 ; i++) {
            randomFile.push_back('a' + rand()%26);
        }
        ofstream tmpFile(randomFile.data());
        if(tmpFile.is_open()) {
            tmpFile << input_cipher << EOF;
            tmpFile.close();
        }
        int c;

        string command = "gpg --passphrase '";
        command.append(this->passPhrase);
        command.append("' --decrypt ");
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
        if(DEBUG) {
            cout << dec << endl;
        }
        int index = 0;

        auto it = std::sregex_iterator(dec.begin(), dec.end(), e);
        for (; it != std::sregex_iterator();
              ++it) {
            for (auto elem : *it) {
                index = it->position() + elem.length();
            }

        }
        dec = dec.substr(index);
        dec.erase(dec.begin()+dec.length()-1);
        dec = ReplaceAll(dec, std::string("\n"), std::string("\\n"));
        return dec;
    }

    bool PGPManager::ImportPub(string pub, string fpr){
        FILE *pipe;
        string randomFile = "";
        int c;
        string output = "";
        struct timeval time;
        
        // logic error 1
        // keyid can also be used for encryption
        // keyid length can be 8 or 16
        // thus malformed fpr can lead to privacy breach
        if(fpr.length() != 40) {
            if(DEBUG) {
                cout << "received length(" + to_string(fpr.length()) + ") malformed fpr/pubkey : " + fpr << endl;
            }
            return false;
        }

        gettimeofday(&time,NULL);
        srand((time.tv_sec*1000)+(time.tv_usec/1000));
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
            exit(0);
        }
        while( (c = fgetc(pipe)) != EOF ) {
            output.push_back(c);
        }
        if(DEBUG || DEMO) {
            cout << output << endl;
        }
        string output_substr = output.substr(9, 8);
        if(DEBUG) {
            cout << output_substr << endl;
        }
        if(!VerifyFprAndPubkey(fpr, output_substr)) {
            return false;
        }
        fclose(pipe);
        return true;
    }

    void PGPManager::ImportKeys(string githubID) {
        FILE *pipe;
        int c;
        string output = "";

        SetTTYEcho(false);
        cout << "Your passphrase :";
        cin >> this->passPhrase;
        SetTTYEcho(true);
        cout << endl;
        string command = "";

        command = "gpg --batch --import ";
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
        if(DEBUG) {
            cout << output << endl;
        }
        output = "";
        command = "gpg --fingerprint " + githubID + " | head -n2 | tail -n1 | tail -c51 | tr -d \" \"";
        pipe = popen(command.c_str(), "r");
        if( pipe == NULL) {
            perror("popen failed\n");
            return;
        }
        while( (c = fgetc(pipe)) != EOF ) {
            output.push_back(c);
        }
        fclose(pipe);
        string fpr = string(output);
        this->fpr = fpr.substr(0, 40);
        if(DEBUG) {
            cout << "[debug] fpr : " << fpr << endl;
            cout << "[debug] fpr.length() : " << to_string(fpr.length()) << endl;
        }

        string testing = "JustForTestString!@#";
        string test_enc = Enc(testing, githubID);
        string test_dec = Dec(test_enc);
        if(testing.compare(test_dec) != 0){
            cout << "Check your PW plz!" << endl;
            exit(1);
        }
    }

    string PGPManager::getFpr() {
        return this->fpr;
    }

    bool PGPManager::VerifyFprAndPubkey(string recvedFpr, string importedKeyID) {
        // logic error 2
        // should compare entire fingerprint to fingerprint
        // comparing keyid with fingerprint can be bypassed

        // compare short key id and fingerprint
        // short key id length : 8
        // fingerprint length : 40
        if(DEBUG) {
            cout << "recvedFpr.substr(32,8) : " << recvedFpr.substr(32,8) << endl;
            cout << "importedKeyID : " << importedKeyID << endl;
        }
        if(importedKeyID.compare(recvedFpr.substr(32,8)) != 0) {
            return false;
        }
        return true;
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
