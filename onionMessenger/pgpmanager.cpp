#include "pgpmanager.h"
#include "common.h"
#include <fstream>
#include <regex>

namespace PGPCrypt{

    PGPManager::PGPManager(){}

    PGPManager::~PGPManager(){}

    std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to){
        size_t start_pos = 0; //string처음부터 검사
        while((start_pos = str.find(from, start_pos)) != std::string::npos)  //from을 찾을 수 없을 때까지
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // 중복검사를 피하고 from.length() > to.length()인 경우를 위해서
        }
        return str;
    }

    string PGPManager::Enc(string input_plain, string recipientID){
        FILE *pipe;
        string enc = "";
        string change_plain = "";
        int c;

        change_plain = ReplaceAll(input_plain, std::string("\""), std::string("\\\""));
        string command = "echo \"";
        command.append(change_plain);
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
        struct timeval tp;
        gettimeofday(&tp, NULL);
        unsigned long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        std::regex e("gpg:\\sencrypted\\swith\\s\\d+-bit\\sRSA\\skey,\\sID\\s[\\w\\d]+,\\screated\\s\\d\\d\\d\\d-\\d\\d-\\d\\d\\n\\s+\".*?\"\\n");
        std::smatch m;

        // srand(time(NULL));   // nope, time() is not enough
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
        // gpg --passphrase "myPassphrase" --decrypt file
        // command injection vulnerability in here
        // POC : '<onionMessenger;/bin/sh>&2;'
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
        // maybe regex better?
        int index = 0;
        //index = dec.find("\"\n");
        //dec = dec.substr(index + 2); // line feed is 1 byte
        auto it = std::sregex_iterator(dec.begin(), dec.end(), e);
        for (; it != std::sregex_iterator();
              ++it) {
            for (auto elem : *it) {
                index = it->position() + elem.length();
                //cout << elem << endl;
            }

        }
        dec = dec.substr(index);
        dec.erase(dec.begin()+dec.length()-1);
        dec = ReplaceAll(dec, std::string("\n"), std::string("\\n"));
        return dec;
    }

    void PGPManager::ImportPub(string pub){
        FILE *pipe;
        string randomFile = "";
        int c;
        string output = "";
        struct timeval time;
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
