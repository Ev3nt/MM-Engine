.686
.MODEL FLAT, C

.DATA
Race_Random BYTE "Default", 0
Race_Unknown BYTE "Unknown", 0

RaceName_Random BYTE "RANDOM", 0
RaceName_Unknown BYTE "UNKNOWN", 0

EXTERN hGame : dword

EXTERN GetRace : proc
EXTERN GetRaceName : proc
EXTERN GetRacesCount : proc
EXTERN GetRacesNamesCount : proc

.CODE

RaceUI PROC
	movzx eax, word ptr [ecx + 28h]
	push eax
	mov eax, hGame
	add eax, 3a1650h
	call eax
	mov eax, [eax + 260h]
	push ebx
	mov ebx, eax
	call GetRacesCount
	cmp ebx, eax
	mov eax, ebx
	pop ebx
	ja pSuccessful1
	lea eax, [eax - 1]
	push eax
	call GetRace
	mov ecx, eax
	pop eax
	jmp pSuccessful2
pSuccessful1:
	lea ecx, Race_Random
pSuccessful2:
	mov eax, hGame
	add eax, 31f510h
	jmp eax
RaceUI ENDP

RaceSounds PROC
	push esi
	mov esi, ecx
	push edx
	call GetRacesCount
	pop edx
	mov ecx, esi
	cmp edx, eax
	mov eax, edx
	ja pSuccessful
	lea eax, [eax - 1]
	push ecx
	push eax
	call GetRace
	mov edx, eax
	pop eax
	pop ecx
	pop esi
	mov eax, hGame
	add eax, 31f530h
	jmp eax
pSuccessful:
	lea ecx, Race_Random
	mov eax, hGame
	add eax, 31f61eh
	jmp eax
RaceSounds ENDP

RaceLoadingScreen PROC
	cmp eax, 64h
	jb pSuccessful1
	sub eax, 64h
pSuccessful1:
	test eax, eax
	je pSuccessful2
	push ebx
	mov ebx, eax
	call GetRacesCount
	cmp ebx, eax
	mov eax, ebx
	pop ebx
	ja pSuccessful3
	lea eax, [eax - 1]
	push eax
	call GetRace
	mov edx, eax
	pop eax
	jmp pSuccessful4
pSuccessful3:
	lea edx, Race_Unknown
	jmp pSuccessful4
pSuccessful2:
	lea edx, Race_Random
pSuccessful4:
	mov eax, hGame
	add eax, 5a3db7h
	jmp eax
RaceLoadingScreen ENDP

RaceName PROC
	test eax, eax
	push esi
	mov esi, ecx
	je pSuccessful1
	push ebx
	mov ebx, eax
	call GetRacesNamesCount
	cmp ebx, eax
	mov eax, ebx
	pop ebx
	ja pSuccessful2
	lea eax, [eax - 1]
	push eax
	call GetRaceName
	mov ecx, eax
	pop eax
	jmp pSuccessful3
pSuccessful2:
	lea ecx, RaceName_Unknown
	jmp pSuccessful3
pSuccessful1:
	lea ecx, RaceName_Random
pSuccessful3:
	mov eax, hGame
	add eax, 58aa46h
	jmp eax
RaceName ENDP

RaceScoreScreen PROC
	push ecx
	call GetRacesCount
	pop ecx
	cmp ecx, eax
	ja pSuccessful
	lea ecx, [ecx - 1]
	push ecx
	call GetRace
	pop ecx
	ret
pSuccessful:
	lea eax, Race_Unknown
	ret
RaceScoreScreen ENDP

RaceOrder PROC
	push ebx
	push esi
	mov ebx, ecx
	mov esi, [ebx + 1a4h]
	mov esi, [esi + 1f0h]
	mov eax, hGame
	add eax, 5595bdh
	jmp eax
RaceOrder ENDP

RaceSlot PROC
	mov eax, [esp + 4]
	mov [ecx + 188h], eax
	mov ecx, [ecx + 1a4h]
	cmp eax, 64h
	jb pSuccessful1
	sub eax, 64h
pSuccessful1:
	cmp edx, 0ch
	jne pSuccessful2
	xor eax, eax
pSuccessful2:
	mov [esp + 4], eax
	mov eax, hGame
	add eax, 6130f0h
	jmp eax
RaceSlot ENDP

RaceStartUnits PROC
	push ebx
	push esi
	push edi
	mov ebx, ecx
	xor edi, edi
pSuccessful7:
	push edi
	mov ecx, ebx
	mov eax, hGame
	add eax, 3a1650h
	call eax
	mov esi, eax
	lea ecx, [esi + 0f0h]
	mov eax, 0
	mov [esi + 260h], eax
	mov eax, hGame
	add eax, 473170h
	call eax
	test eax, eax
	je pSuccessful1
	mov eax, [esi + 25ch]
	and eax, 40h
	or eax, 1
	mov [esi + 25ch], eax
pSuccessful1:
	mov ecx, [esi + 25ch]
	jmp pSuccessful2
pSuccessful3:
	test ecx, ecx
	jne pSuccessful4
	mov ecx, hGame
	add ecx, 0ab73d8h
	mov ecx, [ecx]
	mov eax, hGame
	add eax, 11390h
	call eax
	push eax
	call GetRacesCount
	mov ecx, eax
	pop eax
	mul ecx
	mov cl, 20h
	mov eax, hGame
	add eax, 7e17a0h
	call eax
	inc eax
	mov [esi + 260h], eax
	jmp pSuccessful5
pSuccessful4:
	xor eax, eax
	jmp pSuccessful6
pSuccessful2:
	cmp ecx, 64h
	jb pSuccessful3
	sub ecx, 64h
	jmp pSuccessful3
pSuccessful6:
	mov [esi + 260h], ecx
pSuccessful5:
	inc edi
	cmp edi, 0ch
	jb pSuccessful7
	pop edi
	pop esi
	pop ebx
	ret
RaceStartUnits ENDP

RaceBlocked PROC
	cmp eax, 64h
	jb pSuccessful
	sub eax, 64h
pSuccessful:
	push eax
	mov ecx, ebp
	mov eax, hGame
	add eax, 599BD2h
	jmp eax
RaceBlocked ENDP

RaceIncreaser PROC
	mov ecx, [esp + 4]
	mov eax, hGame
	add eax, 3bd4d0h
	call eax
	test eax, eax
	je pSuccessful1
	cmp dword ptr [esp + 8], 0
	je pSuccessful2
	add dword ptr [eax + 25ch], 64h
	ret
pSuccessful2:
	and dword ptr [eax + 25ch], -41h
pSuccessful1:
	ret
RaceIncreaser ENDP

RaceInit PROC
	mov ecx, [esp + 4]
	mov eax, hGame
	add eax, 3bd4d0h
	call eax
	test eax, eax
	je pSuccessful
	mov edx, [eax + 25ch]
	mov ecx, [esp + 8]
	mov edx, ecx
	mov [eax + 25ch], edx
pSuccessful:
	ret
RaceInit ENDP
END