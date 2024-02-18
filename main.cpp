// Encryption ToolKit
#include <iostream>
#include <bitset>
#include <sstream>
#include <unistd.h>

std::string toBitAndEncrypt(std::string inputString) {
	std::string preBin = "";
    for (char c : inputString) {
        std::bitset<8> bits(c); // bitset is container with size 8
		preBin += bits.to_string();
    }
	std::string finalBin;
	for (char c : preBin) {
		int binInt = c - '0';
		finalBin += std::to_string(binInt + 4);
	}
	return finalBin;
}

std::string toStringAndDecrypt(std::string inputString) {
	std::string finalBin = "";
	std::string preBin = "";
	for (char c : inputString) {
		int binInt = c - '0';
		preBin += std::to_string(binInt - 4);
	}
	int i = 1;
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

int main() {
	std::cout << "  ______                                   __              ________                   __  __    __  __    __     \n /      \\                                 /  |            /        |                 /  |/  |  /  |/  |  /  |    \n/$$$$$$  |  ______   __    __   ______   _$$ |_     ______$$$$$$$$/______    ______  $$ |$$ | /$$/ $$/  _$$ |_   \n$$ |  $$/  /      \\ /  |  /  | /      \\ / $$   |   /      \\  $$ | /      \\  /      \\ $$ |$$ |/$$/  /  |/ $$   |  \n$$ |      /$$$$$$  |$$ |  $$ |/$$$$$$  |$$$$$$/   /$$$$$$  | $$ |/$$$$$$  |/$$$$$$  |$$ |$$  $$<   $$ |$$$$$$/   \n$$ |   __ $$ |  $$/ $$ |  $$ |$$ |  $$ |  $$ | __ $$ |  $$ | $$ |$$ |  $$ |$$ |  $$ |$$ |$$$$$  \\  $$ |  $$ | __ \n$$ \\__/  |$$ |      $$ \\__$$ |$$ |__$$ |  $$ |/  |$$ \\__$$ | $$ |$$ \\__$$ |$$ \\__$$ |$$ |$$ |$$  \\ $$ |  $$ |/  |\n$$    $$/ $$ |      $$    $$ |$$    $$/   $$  $$/ $$    $$/  $$ |$$    $$/ $$    $$/ $$ |$$ | $$  |$$ |  $$  $$/ \n $$$$$$/  $$/        $$$$$$$ |$$$$$$$/     $$$$/   $$$$$$/   $$/  $$$$$$/   $$$$$$/  $$/ $$/   $$/ $$/    $$$$/  \n                    /  \\__$$ |$$ |                                                                               \n                    $$    $$/ $$ |                                                                               \n                     $$$$$$/  $$/                                                                                \n";
	short int selectDecEnc;
	short int selectTarget;
	std::cout << "Select target:\n[1] Message\n[2] File (soon)\n[3] Generate key (Soon)\n: ";
	std::cin >> selectTarget;
	std::cout << "Select method:\n[1] Encrypt\n[2] Decrypt\n: ";
	std::cin >> selectDecEnc;
	if (selectDecEnc == 1 && selectTarget == 1) {
		std::string inputString = "";
		std::cout << "String to encrypt: ";
		std::cin.ignore(); 
		std::getline(std::cin, inputString);
		std::cout << toBitAndEncrypt(inputString);
	}
	else if (selectDecEnc == 2 && selectTarget == 1){
		std::string inputString = "";
		std::cout << "String to decrypt: ";
		std::cin.ignore(); 
		std::getline(std::cin, inputString);
		std::cout << toStringAndDecrypt(inputString);
	}
	else {main(); return 0;}

    return 0;
}