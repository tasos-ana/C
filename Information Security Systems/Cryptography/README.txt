Basistika ston skeleto pou mas exei dwthei.
Ekana kapoies allages se return values apo kapoia functions (ecrypt,decrypt,generate)
wste na epistrefw to length h -1 an kati paei strava.

H genikh idea einai afth pou perigrafete sto lab1. 
	- Kanoume generate ena key me basi to pw kai to bit mode pou mas dwthike
	- Me basi afto to kleidi kanoume ecrypt to input file, afou prwta to diabasoume
		kai stin sunexeia grafoume to cipher text pou paraxthike sto output.
	- Gia to decryption afou exoume kanei pali generate to key, diabazoume to input
		file to opoio tha einai to cipher text mas kai stin sinexeia to kanoume decrypt
		an den uparksei kapoio sfalma tote grafoume to plaintext sto output
	- Gia tin eisagwgh tis upografis afou exoume diabasei to plaintext, to kanoume encrypt 
		me to key pou exoume eksagei. Upologizoume to Mac apo to plaintext. Telos kanoume write
		to cipher text sto output kai stin sunexei append.
	- Gia na kanoume verify diavazoume to cipher text apo to input, eksagoume ta teleftaia 16bit (cmac1)
		stin sunexeia kanoume decrypt to arxeio me to key poy mas exw dothei. Upologizoume to cmac
		apo to plaintext pou proekupse. Telos kanoume compare afta ta 2 cmac.

Task F:
	4) 	Kai ta duo arxeia "hy457_verifyme_256.txt" & "hy457_verifyme_128.txt" 
		den exoun valid CMAC
