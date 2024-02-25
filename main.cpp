#include <iostream>
#include <bitset>
#include <fstream>
#include <random>
#include <bigint.h>
int genRandom(int from, int upto) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> dist(from, upto);
	return dist(gen);
}
bigint gcdExtended(bigint a, bigint b, bigint* x, bigint* y) {
    if (a == bigint(0)) {
        *x = 0;
        *y = 1;
        return b;
    }

    bigint x1, y1;
    bigint gcd = gcdExtended(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}
bigint modInverse(bigint e, bigint phiN) {
	bigint x, y;
    gcdExtended(e, phiN, &x, &y);
    bigint d = (x % phiN + phiN) % phiN;
    return d;
}
void generateRsaKey() {
	char numslist[] = "0123456789";
	bigint num1(1);
	bigint num2(1);
	bigint e(1);
	while (true) { // gen num1
		std::string prenum = "1";
		for (int i = 0; i<genRandom(64, 256); i++) { // set how many int in num, example 60
			prenum = prenum + numslist[genRandom(0, 9)];
		}
		num1 = prenum;
		if (num1 % bigint(2) != bigint(0) && num1 % bigint(3) != bigint(0) &&  num1 % bigint(5) != bigint(0) && num1 % bigint(7) != bigint(0)) {break;}
		else {continue;}
	}
	while (true) { // gen num2
		std::string prenum = "1";
		for (int i = 0; i<genRandom(64, 256); i++) { // set how many int in num, example 60
			prenum = prenum + numslist[genRandom(0, 9)];
		}
		num2 = prenum;
		if (num2 % bigint(2) != bigint(0) && num2 % bigint(3) != bigint(0) &&  num2 % bigint(5) != bigint(0) && num2 % bigint(7) != bigint(0)) {break;}
		else {continue;}
	}
	bigint N(num1*num2); // set N
	bigint phiN = (num1-bigint(1)) * (num2-bigint(1)); //set phi(N)
	while (true) { // gen e
		std::string prenum = "1";
		for (int i = 0; i<genRandom(64, 256); i++) { // set how many int in num, example 60
			prenum = prenum + numslist[genRandom(0, 9)];
		}
		e = prenum;
		if (e % bigint(2) != bigint(0) && e % bigint(3) != bigint(0) &&  e % bigint(5) != bigint(0) && e % bigint(7) != bigint(0) && e < phiN) {break;}
		else {continue;}
	}
	bigint d(modInverse(e, phiN));
	std::cout << "\nPrivate key: {" << d << ", " << N << "}\nPublic key: {" << e << ", " << N << '\n';
}

std::string toBitAndEncrypt(std::string inputString, std::string key) {
	std::string toChars16bit = "";
	std::string bitsAfterCrypting = "";
	int i = 0;
	for (char c : inputString) { // every char in message
		std::bitset<16> bits(c); // bitset is container with size 16
		for (int u = 0; u < key.length(); u++) { // bitset in key char
			std::bitset<16> keyCharToBit(key[u]);
			std::bitset<16> indexBitsPool(i);
			bits = bits ^ keyCharToBit;
			for (int l = 0; l < 16; l++) {
				if (keyCharToBit[l] == 1) {
					bits = bits ^ keyCharToBit;
					bits = bits ^ indexBitsPool;
				}
			}
		}
		bitsAfterCrypting = bitsAfterCrypting + bits.to_string();
		i++;
	} // SPOOFING
	std::bitset<16> keyCharToBit(key[0]); // first char of key give seed info
	unsigned short int counterOfSpoofBytes = 2;
	for (int y = 0; y < keyCharToBit.size(); y++) {
		if (keyCharToBit[y]) {counterOfSpoofBytes++;};
	}
	if (keyCharToBit[3] == 1) { // add spoof bits to begin if 4 bit == 1
		for(int h = 0; h < counterOfSpoofBytes * 16; h++) {bitsAfterCrypting = std::to_string(genRandom(0,1)) + bitsAfterCrypting;}
	} else { // add to end
		for(int h = 0; h < counterOfSpoofBytes * 16; h++) {bitsAfterCrypting = bitsAfterCrypting + std::to_string(genRandom(0,1));}
	}
	for (int b = 0; b < bitsAfterCrypting.length(); b = b+16) {
		std::string bitsPreset = "";
		for (int j = 0; j < 16; j++) {
			bitsPreset = bitsPreset + bitsAfterCrypting[b+j];
		}
		std::bitset<16> bits(bitsPreset);
		toChars16bit = toChars16bit + static_cast<char>(bits.to_ulong());
	}
	return toChars16bit;
}

std::string toStringAndDecrypt(std::string inputString, std::string key) {
	std::string toCharFromBin = "";
	std::string bitsAfterCrypting = "";
	for (int i = 0; i < inputString.length(); i++) { // every char in encrypted message
		std::bitset<16> bits(inputString[i]); // convert char to bitset
		for (int u = 0; u < key.length(); u++) { // bitset in key char
			std::bitset<16> keyCharToBit(key[u]);
			std::bitset<16> indexBitsPool(i);
			bits = bits ^ keyCharToBit;
			for (int l = 0; l < 16; l++) {
				if (keyCharToBit[l] == 1) {
					bits = bits ^ keyCharToBit;
					bits = bits ^ indexBitsPool;
				}
			}
		}
		bitsAfterCrypting = bitsAfterCrypting + bits.to_string();
	} // CLEAN SPOOFED
	std::bitset<16> keyCharToBit(key[0]); // first char of key give seed info
	unsigned short int counterOfSpoofBytes = 2;
	for (int y = 0; y < 16; y++) {
		if (keyCharToBit[y]) {counterOfSpoofBytes++;};
	}
	if (keyCharToBit[3] == 1) { // remove spoof bits from begin
		for(int h = 0; h < counterOfSpoofBytes; h++) {bitsAfterCrypting = bitsAfterCrypting.substr(16, bitsAfterCrypting.length()-1);}
	} else { // remove from end
		for(int h = 0; h < counterOfSpoofBytes; h++) {bitsAfterCrypting = bitsAfterCrypting.substr(0, bitsAfterCrypting.length()-16);}
	}
	for (int b = 0; b < bitsAfterCrypting.length(); b = b+16) {
		std::string bitsPreset = "";
		for (int j = 0; j < 16; j++) {
			bitsPreset = bitsPreset + bitsAfterCrypting[b+j];
		}
		std::bitset<16> bits(bitsPreset);
		toCharFromBin = toCharFromBin + static_cast<char>(bits.to_ulong());
	}
	return toCharFromBin;
}

void cryptFile (std::string path, std::string key) {
	std::fstream file(path);
	std::string fileLegacyData;
	file >> fileLegacyData;
	file.close();
	std::string encryptedFileData = toBitAndEncrypt(fileLegacyData, key);
	std::fstream filein(path);
	filein << encryptedFileData;
	filein.close();
}

void DecryptFile (std::string path, std::string key) {
	std::fstream file(path);
	std::string fileEncData;
	file >> fileEncData;
	file.close();
	std::string legacyFileData = toStringAndDecrypt(fileEncData, key);
	std::fstream filein(path);
	filein << legacyFileData;
	filein.close();
}

std::string genKey(long int keysize) {
	char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/[]()-+_%#@!/\\;,.";
	std::string key = "";
	std::random_device rand;
	std::mt19937 rng(rand());
	for (int i = 0; i < keysize; i++) {
		std::uniform_int_distribution<int> dist(0,sizeof(alphabet)-1);
		key = key + alphabet[dist(rand)];
	}
	return key;
}

int main() {
	std::cout << "  ______                                   __              ________                   __  __    __  __    __     \n /      \\                                 /  |            /        |                 /  |/  |  /  |/  |  /  |    \n/$$$$$$  |  ______   __    __   ______   _$$ |_     ______$$$$$$$$/______    ______  $$ |$$ | /$$/ $$/  _$$ |_   \n$$ |  $$/  /      \\ /  |  /  | /      \\ / $$   |   /      \\  $$ | /      \\  /      \\ $$ |$$ |/$$/  /  |/ $$   |  \n$$ |      /$$$$$$  |$$ |  $$ |/$$$$$$  |$$$$$$/   /$$$$$$  | $$ |/$$$$$$  |/$$$$$$  |$$ |$$  $$<   $$ |$$$$$$/   \n$$ |   __ $$ |  $$/ $$ |  $$ |$$ |  $$ |  $$ | __ $$ |  $$ | $$ |$$ |  $$ |$$ |  $$ |$$ |$$$$$  \\  $$ |  $$ | __ \n$$ \\__/  |$$ |      $$ \\__$$ |$$ |__$$ |  $$ |/  |$$ \\__$$ | $$ |$$ \\__$$ |$$ \\__$$ |$$ |$$ |$$  \\ $$ |  $$ |/  |\n$$    $$/ $$ |      $$    $$ |$$    $$/   $$  $$/ $$    $$/  $$ |$$    $$/ $$    $$/ $$ |$$ | $$  |$$ |  $$  $$/ \n $$$$$$/  $$/        $$$$$$$ |$$$$$$$/     $$$$/   $$$$$$/   $$/  $$$$$$/   $$$$$$/  $$/ $$/   $$/ $$/    $$$$/  \n                    /  \\__$$ |$$ |                                                                               \n                    $$    $$/ $$ |                                                                               \n                     $$$$$$/  $$/                                                                                \n";
	short int selectDecEnc;
	short int selectTarget;
	std::string key;
	std::cout << "Select target:\n[1] Message\n[2] File\n[3] Generate key\n[4] RSA menu\n: ";
	std::cin >> selectTarget;
	if (selectTarget < 3) {
		std::cout << "Select method:\n[1] Encrypt\n[2] Decrypt\n: ";
		std::cin >> selectDecEnc;
	}
	if (selectTarget < 3) { // set key
		std::cout << "Set key (password 16+ chars): ";
		std::cin >> key;
	}
	if (selectDecEnc == 1 && selectTarget == 1) { // message crypt
		std::string inputString = "";
		std::cout << "String to encrypt: ";
		std::cin.ignore();
		std::getline(std::cin, inputString);
		std::cout << "\nEncrypted message: '" << toBitAndEncrypt(inputString, key) << "'\n";
		system("pause");
	}
	else if (selectDecEnc == 2 && selectTarget == 1){ // file crypt
		std::string inputString = "";
		std::cout << "String to decrypt: ";
		std::cin.ignore(); 
		std::getline(std::cin, inputString);
		std::cout << "\nDecrypted message: " << toStringAndDecrypt(inputString, key) << '\n';
		system("pause");
	}
	else if (selectDecEnc ==1 && selectTarget == 2) { // message decrypt
		char confirm = 'n';
		std::string inputString = "";
		std::cout << "File path which crypt: ";
		std::cin.ignore();
		std::getline(std::cin, inputString);
		std::cout << "\nConfirm overwrite file | [y] yes [n] no: ";
		std::cin >> confirm;
		if (confirm == 'y') {
			cryptFile(inputString, key);
			std::cout << "\nFile succefuly encrypted!\n";
		} else {std::cout << "\nAbort\n";}
		system("pause");
	}
	else if (selectDecEnc == 2 && selectTarget == 2) { // file decrypt
		char confirm = 'n';
		std::string inputString = "";
		std::cout << "File path which decrypt: ";
		std::cin.ignore();
		std::getline(std::cin, inputString);
		std::cout << "\nConfirm overwrite file | [y] yes [n] no: ";
		std::cin >> confirm;
		if (confirm == 'y') {
			DecryptFile(inputString, key);
			std::cout << "\nFile succefuly decrypted!\n";
		} else {std::cout << "\nAbort\n";}
		system("pause");
	}
	else if (selectTarget == 4) { // menu rsa
		unsigned short int chooseMethod = 0;
		std::cout << "Select RSA operation\n[1] Generate key pair\n[2] Message Enc/Dec\n[3] File Enc/Dec\n: ";
		std::cin >> chooseMethod;
		if (chooseMethod == 1) {
			generateRsaKey();
		}
	}
	else if (selectTarget == 3) { // gen key
		long int* keysize = new long int;
		std::cout << "Set keysize: ";
		std::cin >> *keysize;
		std::string key = genKey(*keysize);
		std::cout << "\nYour key: " + key + "\n";
		system("pause");
	}
	else {main(); return 0;}
    return 0;
}
