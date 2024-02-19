// Encryption ToolKit
#include <iostream>
#include <bitset>
#include <fstream>
#include <random>
void swapBits(int &bit) {
	if (bit == 1) {bit = 0;}
	else {bit = 1;}
}
std::string toBitAndEncrypt(std::string inputString, std::string key) {
	std::string preBin = "";
	std::string cryptedBin = "";
    for (char c : inputString) {
        std::bitset<8> bits(c); // bitset is container with size 8
		preBin += bits.to_string();
    }
	std::string finalBin;
	int i = 0;
	while (i < preBin.length()) {
		int binInt = preBin[i] - '0';
		int s = 0;
		int bitCangesounter = 0;
		while (s < key.length()) { // [ENCRYPTION] encrypting every byte
			std::bitset<8> bits(key[s]);
			if (bits[0] == 1) {
				swapBits(binInt);
				bitCangesounter += 1;
			}
			else if (bits[1] == 1) {
				swapBits(binInt);
				bitCangesounter += 1;
			}
			else if (bits[2] == 1) {
				swapBits(binInt);
				bitCangesounter += 1;
			}
			else if (bits[3] == 1) {
				swapBits(binInt);
				bitCangesounter += 1;
			}
			s++; // next key char
		}
		finalBin = finalBin + std::to_string(binInt);
		i++; // next bit
	}
	std::string toChars8bit = "";
	i = 0;
	int u = 0;
    while (i*8 <= finalBin.length()){
		std::string binarySequence = "";
		binarySequence = "";
		while (u < i*8) {
			binarySequence += finalBin[u];
			u++;
		}
        std::bitset<8> bits(binarySequence); // bitset is container with size 8
		toChars8bit += static_cast<char>(bits.to_ulong());
		i++;
    }
	return toChars8bit;
}

std::string toStringAndDecrypt(std::string inputString, std::string key) {
	std::string finalBin = "";
	std::string preBin = "";
	std::string toBit8 = "";
	int i = 0;
	for (char c : inputString) {
        std::bitset<8> bits(c); // bitset is container with size 8
		toBit8 = toBit8 + bits.to_string();
		i++;
    }
	int o = 0;
	while (o < toBit8.length()) {
		int binInt = toBit8[o] - '0';
		int s = 0;
		while (s < key.length()) { // decrypting every byte
			std::bitset<8> bits(key[s]);
			if (bits[0] == 1) {
				swapBits(binInt);
			}
			if (bits[1] == 1) {
				swapBits(binInt);
			}
			if (bits[2] == 1) {
				swapBits(binInt);
			}
			if (bits[3] == 1) {
				swapBits(binInt);
			}
			s++; // next key char
		}
		preBin = preBin + std::to_string(binInt);
		o++;
	}
	i = 1;
	int u = 0;
    while (i*8 <= preBin.length()){
		std::string binarySequence = "";
		binarySequence = "";
		while (u < i*8) {
			binarySequence += preBin[u];
			u++;
		}
        std::bitset<8> bits(binarySequence); // bitset is container with size 8
		finalBin += static_cast<char>(bits.to_ulong());
		i++;
    }
	return finalBin;
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

std::string genKey() {
	char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	char key[64] = "";
	std::random_device rand;
	std::mt19937 rng(rand());
	for (int i = 0; i<64; i++) {
		std::uniform_int_distribution<int> dist(1,62);
		key[i] = alphabet[dist(rand)];
	}
	return key;
}

int main() {
	std::cout << "  ______                                   __              ________                   __  __    __  __    __     \n /      \\                                 /  |            /        |                 /  |/  |  /  |/  |  /  |    \n/$$$$$$  |  ______   __    __   ______   _$$ |_     ______$$$$$$$$/______    ______  $$ |$$ | /$$/ $$/  _$$ |_   \n$$ |  $$/  /      \\ /  |  /  | /      \\ / $$   |   /      \\  $$ | /      \\  /      \\ $$ |$$ |/$$/  /  |/ $$   |  \n$$ |      /$$$$$$  |$$ |  $$ |/$$$$$$  |$$$$$$/   /$$$$$$  | $$ |/$$$$$$  |/$$$$$$  |$$ |$$  $$<   $$ |$$$$$$/   \n$$ |   __ $$ |  $$/ $$ |  $$ |$$ |  $$ |  $$ | __ $$ |  $$ | $$ |$$ |  $$ |$$ |  $$ |$$ |$$$$$  \\  $$ |  $$ | __ \n$$ \\__/  |$$ |      $$ \\__$$ |$$ |__$$ |  $$ |/  |$$ \\__$$ | $$ |$$ \\__$$ |$$ \\__$$ |$$ |$$ |$$  \\ $$ |  $$ |/  |\n$$    $$/ $$ |      $$    $$ |$$    $$/   $$  $$/ $$    $$/  $$ |$$    $$/ $$    $$/ $$ |$$ | $$  |$$ |  $$  $$/ \n $$$$$$/  $$/        $$$$$$$ |$$$$$$$/     $$$$/   $$$$$$/   $$/  $$$$$$/   $$$$$$/  $$/ $$/   $$/ $$/    $$$$/  \n                    /  \\__$$ |$$ |                                                                               \n                    $$    $$/ $$ |                                                                               \n                     $$$$$$/  $$/                                                                                \n";
	short int selectDecEnc;
	short int selectTarget;
	std::string key;
	std::cout << "Select target:\n[1] Message\n[2] File\n[3] Generate key (Soon)\n: ";
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
		std::cout << "\nEncrypted message (excluding forgings!): '" << toBitAndEncrypt(inputString, key) << "'\n";
	}
	else if (selectDecEnc == 2 && selectTarget == 1){
		std::string inputString = "";
		std::cout << "String to decrypt: ";
		std::cin.ignore(); 
		std::getline(std::cin, inputString);
		std::cout << "\nDecrypted message: " << toStringAndDecrypt(inputString, key);
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
	}
	else if (selectTarget == 3) {
		std::string key = genKey();
		std::cout << "\nYour key: " + key + "\n";
	}
	else {main(); return 0;}

    return 0;
}
