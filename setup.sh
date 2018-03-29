cp testkey/* onionMessenger
make -C onionMessenger dist
make -C onionMessenger
mkdir ~/.gnupg
echo 'no-tty' >> ~/.gnupg/gpg.conf
