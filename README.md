						TASK 1 - 6: EPOCH CCONVERTER
---------------------------------------------------------------------------------------------------------------------------------

	In aceste task-uri am simulat functionalitatea unui epoch converter printr-o multitudine de functii care transforma un timestamp intr-o data si intr-o ora prin intermediul unori structuri care retin aceste variabile. Am transformat dintr-un timestamp intr-o data, dintr-o data intr-un timestamp si overall am invatat cum se lucreaza cu timestamp-uri.
		TASK 1: Prin intermediul unor operatii am transformat un timestamp intr-o ora in format ora:min:sec
		TASK 2: Cu ajutorul unui while am transformat un timestamp intr-o data fara sa tin cont de anii bisecti
		TASK 3: La fel, prin intermediul unui while, dar de data asta tinand cont de zilele lunii si de anii bisecti, am transformat un timestamp intr-o data de forma year:month:day
		TASK 4: Folosindu-ma de functiile de la task 1 si 3 am transformat un timestamp intr-o data completa
		TASK 5: In acest task am convertit o data intr-un timestamp tinand cont de timezone
		TASK 6: O functie de afisare a unei dati sub formatul zi,luna,an, ora:min:sec UTC+/-diferenta_timezone
  

  
					TASK 7 - 8: BASIC / COMPLEX NETWORK PACKING
---------------------------------------------------------------------------------------------------------------------------------

	In aceste 2 task-uri am invatat cum se realizeaza memorarea datelor pe biti intr-un mod simplu, folosind primii 15 biti dintr-un unsigned int, dar si intr-un mod complex, memorand datele pe bitii unor unsigned int-uri intr-un mod continuu, fara sa pierdem vreun bit si pentru un plus de realism am folosit si conceptul unui control bit care ne spune care unsigned int-uri sunt corupte si care sunt bune.
	TASK 7: Basic Network Packing, datele sunt stocate pe primii 15 biti ai unui unsigned int. In acest task, am scos datele, le-am sortat si le-am afisat.
	TASK 8: Complex Network Packing, datele sunt stocate de-a lungul mai multor biti. Pentru realizarea acestui task am ales ideea de a creea un buffer care retine bitii care urmeza sa fie folositi pentru extragerea unei date, o variabila care tine minte cati biti am citit si cati mai sunt de citit. Pentru acel plus de realism, am tinut cont pentru fiecare data de unsigned int-ul in care se afla prin intermediul a 2 variabile, in acest mod am verificat si pentru datele care sunt in acelasi unsigned int dar si pentru datele care sunt distribuite pe 2 unsigned int-uri bit-ul de control. Dupa verificarea bitului de control, toate datele care nu erau "corupte" le-am adaugat intr-un vector pe care l-am sortat in oridine cronologica si dupa le-am afisat.
 
 
	
					TASK 9: MULTI - TIMEZONE EVENT PLANNER
---------------------------------------------------------------------------------------------------------------------------------

	Acest task a fost unul interactiv, in care am folosit functiile create anterior la task-urile 1 - 6 pentru a gasii primul eveniment de o durata care este citia de la tastatura si un numar minim de persoane care trebuie sa participe la acel eveniment, la fel, citit de la tastatura. 
	Pentru realizarea acestui task am transformat in timestamp si retinut pentru fiecare persoana inceputul si finalul intervalelor in care erau liberi. Apoi, pentru fiecare persoana, am unit intervalele care se suprapuneau. Dupa, am pus intr-un vector timestamp-ul ce reprezenta inceputul unui interval pentru fiecare persoana, am sortat acest vector si am scapat de dubluri, ramanand cu un vector de timestamp-uri de inceput, ordonat, fara nicio dublura. 
		Dupa, am iterat cate un timestamp din acel vector prin fiecare persoana si fiecare interval verificand daca e mai mare ca inceputul si daca el plus durata evenimentului e mai mic ca finalul intervalului. Daca era, numaram intr-o variabila acest fapt pe care mai apoi o comparam cu numarul minim de persoana. Daca variabila era mai mare sau egala cu numarul minim de persoana inseamna ca am gasit un interval bun pe care il salvez si dupa am dat break. Faptul ca vectorul de timestamp-uri era ordonat mi-a oferit siguranta ca acesta este primul timestamp.
		Cu timestamp-ul care satisface conditiile gasit, am ordonat prima data dupa nume personele si dupa, iterand prin fiecare persoana si fiecare interval al persoanei respective, am verificat daca timestamp-ul se afla in acel interval, afisandu-l mai apoi daca se afla. Faptul ca am unit intervalele care se suprapuneau imi ofera siguranta ca aceasta verificare e suficenta si corecta.
		Daca timestamp-ul gasit nu se afla in niciun interval al unei persoane, se va afisa pentru persoana respectiva mesajul "invalid", iar daca nu se gaseste niciun timestamp care sa satisfaca conditiile se va afisa mesajul "imposibil".
