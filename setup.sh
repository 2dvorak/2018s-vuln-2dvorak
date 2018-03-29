cp ../testkey/* ./
mkdir .obj
make
mkdir ~/.gnupg
echo 'no-tty' >> ~/.gnupg/gpg.conf
