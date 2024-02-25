#include <iostream>
#include <bitset>
#include <fstream>
#include <random>
void swapBits(int &bit) {
	if (bit == 1) {bit = 0;}
	else if (bit == 0) {bit = 1;}
}
std::string toBitAndEncrypt(std::string inputString, std::string key) {
	std::string toChars8bit = "";
	std::string preBin = "";
	for (char c : inputString) { // every char in message
		std::bitset<8> bits(c); // bitset is container with size 8
		for (int u = 0; u < key.length(); u++) { // bitset in key char
			std::bitset<8> keyCharToBit(key[u]);
			bits = bits ^ keyCharToBit;
		}
		toChars8bit = toChars8bit + static_cast<char>(bits.to_ulong());
	}
	return toChars8bit;
}

std::string toStringAndDecrypt(std::string inputString, std::string key) {
	std::string toCharFromBin = "";
	for (int i = 0; i < inputString.length(); i++) { // every char in encrypted message
		std::bitset<8> bits(inputString[i]); // convert char to bitset
		for (int u = 0; u < key.length(); u++) { // bitset in key char
			std::bitset<8> keyCharToBit(key[u]);
			bits = bits ^ keyCharToBit;
		}
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
	char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/[]()-+";
	std::string key = "";
	std::random_device rand;
	std::mt19937 rng(rand());
	for (int i = 0; i < keysize; i++) {
		std::uniform_int_distribution<int> dist(0,68);
		key = key + alphabet[dist(rand)];
	}
	return key;
}

int main() {
	std::cout << "  ______                                   __              ________                   __  __    __  __    __     \n /      \\                                 /  |            /        |                 /  |/  |  /  |/  |  /  |    \n/$$$$$$  |  ______   __    __   ______   _$$ |_     ______$$$$$$$$/______    ______  $$ |$$ | /$$/ $$/  _$$ |_   \n$$ |  $$/  /      \\ /  |  /  | /      \\ / $$   |   /      \\  $$ | /      \\  /      \\ $$ |$$ |/$$/  /  |/ $$   |  \n$$ |      /$$$$$$  |$$ |  $$ |/$$$$$$  |$$$$$$/   /$$$$$$  | $$ |/$$$$$$  |/$$$$$$  |$$ |$$  $$<   $$ |$$$$$$/   \n$$ |   __ $$ |  $$/ $$ |  $$ |$$ |  $$ |  $$ | __ $$ |  $$ | $$ |$$ |  $$ |$$ |  $$ |$$ |$$$$$  \\  $$ |  $$ | __ \n$$ \\__/  |$$ |      $$ \\__$$ |$$ |__$$ |  $$ |/  |$$ \\__$$ | $$ |$$ \\__$$ |$$ \\__$$ |$$ |$$ |$$  \\ $$ |  $$ |/  |\n$$    $$/ $$ |      $$    $$ |$$    $$/   $$  $$/ $$    $$/  $$ |$$    $$/ $$    $$/ $$ |$$ | $$  |$$ |  $$  $$/ \n $$$$$$/  $$/        $$$$$$$ |$$$$$$$/     $$$$/   $$$$$$/   $$/  $$$$$$/   $$$$$$/  $$/ $$/   $$/ $$/    $$$$/  \n                    /  \\__$$ |$$ |                                                                               \n                    $$    $$/ $$ |                                                                               \n                     $$$$$$/  $$/                                                                                \n";
	short int selectDecEnc;
	short int selectTarget;
	std::string key;
	std::cout << "Select target:\n[1] Message\n[2] File\n[3] Generate key\n: ";
	std::cin >> selectTarget;
	if (selectTarget != 3) {
		std::cout << "Select method:\n[1] Encrypt\n[2] Decrypt\n: ";
		std::cin >> selectDecEnc;
	}
	if (selectTarget != 3) {
		std::cout << "Set key (password 8+ chars): ";
		std::cin >> key;
	}
	if (selectDecEnc == 1 && selectTarget == 1) {
		std::string inputString = "";
		std::cout << "String to encrypt: ";
		std::cin.ignore();
		std::getline(std::cin, inputString);
		std::cout << "\nEncrypted message: '" << toBitAndEncrypt(inputString, key) << "'\n";
		system("pause");
	}
	else if (selectDecEnc == 2 && selectTarget == 1){
		std::string inputString = "";
		std::cout << "String to decrypt: ";
		std::cin.ignore(); 
		std::getline(std::cin, inputString);
		std::cout << "\nDecrypted message: " << toStringAndDecrypt(inputString, key) << '\n';
		system("pause");
	}
	else if (selectDecEnc ==1 && selectTarget == 2) {
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
	else if (selectDecEnc ==2 && selectTarget == 2) {
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
	else if (selectTarget == 3) {
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
