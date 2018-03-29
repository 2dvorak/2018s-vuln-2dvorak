cp ../testkey/* ./
mkdir -p .obj
make
mkdir ~/.gnupg
echo 'no-tty' >> ~/.gnupg/gpg.conf
