# snipHer

- TA : SeongIl Wi
- Seungyeop Lee

---

### Her Messenger is back
![Her](image/Her.jpg)

---

### ..with More Security!!
#### SEHer Messenger

![SEHer](image/SEHer.jpg)

- Security Enhanced Her Messenger

- 더욱 안전해 더더욱 믿고 쓰는 SEHer 메신저

---

### Enhanced Security

- 암호화시 Fingerprint 사용 : Key ID 사용으로 인한 충돌 방지

- Public Key와 Fingerprint 검증 : 검증되지 않은 사용자 거절

---

### Fingerprint?

![Fpr](image/fingerprint.jpg)

---

#### Happy Hacking? Nah.
### Happy Secure Messaging!
#### with SEHer Messenger

---

### ...not anymore.
![snipHer](image/snipHer.jpg)

---

### snipHer
- Automated Sniffing
- Stealthy Sniffing
- Anonymous Sniffing

#### how?

---

### SEHer's Encryption Scheme

```
$ gpg --encrypt -r "recipient's FPR"
```
---

### But What If?
```
$ gpg --encrypt -r "recipient's FPR" -r "attacker's FPR"
```

---

![happened](image/happen.png)

---

### "Key Protocol"

- SEHer protocol to inform login/logout to others
- Also, used for public key exchange
- and the Fingerprint

---

### Verify Received Fingerprint

Valid and Correct Fingerprint?

---

### Fingerprint Length?

```c++
if(fpr.length() != 40) {
	return false;
}
```

---

### Fingerprint match Public Key?

```c++
if(!VerifyFprAndPubkey(fpr, keyid)) {
    return false; \
}
```

---

### Fingerprint match Public Key?
<div style="text-align:left;">
<h4> Use gpg's output</h4>
</div>

```shell
$ gpg --import pubkey.pub
gpg: key D9595B48: public key "githubB" imported
gpg: Total number processed: 1
gpg:               imported: 1  (RSA: 1)
```

@[2]
#### Returns "Short Key ID"

---

### Fingerprint match Public Key?
<div style="text-align:left;">
<h4> Compare Key ID vs. Fingerprint</h4>
</div>

```c++
// compare short key id and fingerprint
// short key id length : 8
// fingerprint length : 40

if(importedKeyID.compare(recvedFpr.substr(32,8)) != 0) {
	return false;
}
return true;
```

---

### Fingerprint match Public Key?
<div style="text-align:left;">
<h4>Compare Key ID vs. Fingerprint</h4>
</div>

![compare](image/compare.jpg)

---

### But What If...??

![misscompare](image/misscompare.jpg)

---

### Verify Received Key Protocol

Really sent by the "Sender" ??

---

### ...Oops!

![maan](image/maan.png)

---

### Logic Errors

- gpg also accepts "Key ID" for --recipient
- SEHer does not verify entire Fingerprint

---

### snipHer

공격자의 Key ID를 사용자들의 Fingerprint에 주입

---

### snipHer

![step1](image/step1.png)

---

### snipHer

![step2](image/step2.png)

---

### snipHer

![step3](image/step3.png)

---

### snipHer

![step4](image/step4.png)

---

### Demo


+++

### Demo Backup Slides..

+++

### Demo Backup Slides..

---

### Thank you

#### Questions are welcomed

