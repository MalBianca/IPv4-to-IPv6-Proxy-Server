#include <stdio.h> //pt intrare /iesire
#include <string.h> //pt siruri
#include <stdlib.h> //pt functii generale
#include <unistd.h> //pt functii POSIX(close,read,write)
#include <netdb.h> //pt functii retea(getaddrinfo,gethostebyname)
#include <sys/socket.h> // functii pt lucrul cu socket-uri(socket,bind,listen,accept)
int main(){
int consocket, new_fd; //declara 2 variabile de tip intreg
//consocket: pt socket ul servarului
//new_fd: pt conexiunile clientului
struct sockaddr_storage client_addr; //pt adr de la clienti (IPV4/IPV6)
socklen_t client_addr_size = sizeof(client_addr); //variabila pt dimensiunea structurii client_addr
char buf[1000]; //declaratia unui buffer de caractere pt a stoca msj primite
struct addrinfo hints, *res; //configurare pt functia getaddrinfo
memset(&hints, 0, sizeof(hints)); //initializeaza structura hints cu 0
hints.ai_family = AF_INET; //specifica utilizarea adr IPV4
hints.ai_socktype = SOCK_STREAM; //protocol TCP
hints.ai_flags = AI_PASSIVE; //seteaza flag-ul AI_PASSIE, serverul va asculta pe orice adresa locala disponibila
if (getaddrinfo(NULL, "22126", &hints, &res) != 0) { //apleaza functia getaddrinfo pt a obtine informatii despre adresa serverului
//null: utilizarea adresei locale implicite, urmat de portul pe care serverul va asculta conexiuni, hints: parametrii de configurare, res: pointer catre lista de struct cu adr rezultate
printf("Eroare la getaddrinfo\n");//afisare mesaj eroare
return 1;//incheie programul cu un cod de eroare
}
consocket = socket(res->ai_family,res->ai_socktype, res->ai_protocol);//crearea unui socket
//res->ai_family: Familia de adrese IPV4
//res->ai_socktype: Tipul de socket (TCP)
//res->ai_protocol: Protocol TCP

if (consocket==-1){ //verifica daca socket ul a fost creat cu succes (-1 indica eroare)
printf("Eroare la creare socket\n"); //afiseaza msj in caz de eroare
return 1; //incheie programul cu cod de eroare
}
printf("Socket creat\n"); //afiseaza msj daca socket-ul a fost creat cu succes
//Asocierea socket-ului(bind) cu adr si portul definite in server_addr
if(bind(consocket, res->ai_addr, res->ai_addrlen) == -1){
printf("Eroare la bind\n"); //afiseaza msj in caz de eroare
close(consocket); //inchide socket
return 1; //inchide programul cu cod de eroare
}
printf("Bind reusit.Port server: 22126\n");//afiseaza msj daca bind-ul a avut succes
//seteaza socket-ul serverului in modul de ascultare
if (listen(consocket,10)<0){ //permite o coada max de 10 conexiuni
printf("Eroare la listen\n");
close(consocket);//inchide socket-ul serverului
return 1; //inchide programul cu cod de eroare
}
printf("Serverul asculta conenexiuni\n");
//bucla principala pt acceptarea si tratarea conexiunilor client
while ((new_fd=accept(consocket,(struct sockaddr *)&client_addr,&client_addr_size))){
if (new_fd==-1){ //verifica daca acceptarea conexiunii a esuat
printf("Eroare la accept\n");
close(consocket); //inchide socket-ul serverului
return 1; //inchide programul cu codul de eroare
}
printf("Conexiune acceptata\n");
int connection_active=1;//flag pt mentinerea conexiunii active

//bucla pt tratarea comenzilor clientului
while(connection_active){
int bytes_recv=recv(new_fd,buf,sizeof(buf)-1,0); //primeste date de la client
//daca clientul s-a deconectat sau apare o eroare
if (bytes_recv<=0){
printf("Clientul s-a deconectat sau eroare la primirea datelor\n");
connection_active = 0; // inchide conexiunea
}else {
buf[bytes_recv]='\0'; //terminator de sir pt msj primit
printf("Raspuns:%s\n",buf);
if(strcmp(buf,"01#")== 0){ //verifica daca msj primit este comanda 01#
printf("Comanda valida:Integrare client IPV6\n");\
int client_sock=socket(AF_INET6,SOCK_STREAM,0); //crearea socket pt IPV6
if(client_sock==-1){ //verifica eroarea la crearea socket-ului
printf("Eroare la creare socket IPV6\n");
continue; //trecerea la urmatoarea iteratie
}
struct addrinfo hints, *res; //configurare pt functia getaddrinfo
memset(&hints, 0, sizeof(hints)); //initializare structura hints cu 0
hints.ai_family = AF_INET6; //specifica utilizarea IPV6
hints.ai_socktype = SOCK_STREAM; //protocol TCP
int status = getaddrinfo("::1", "80", &hints, &res); //rezolva adr "::1" si port "80"
if (status != 0){ //Daca apare o eroare la getaddrinfo
printf("Eroare la gettaddrinfo\n");
close(client_sock);//inchde socket-ul
continue; //trecere la urmatoarea iteriatie
}

if (connect(client_sock, res->ai_addr, res->ai_addrlen) == -1) {//incearca conectarea la serverul IPV6.
printf("Eroare la conectare la website IPV6\n");
close(client_sock);//inchide socket
continue;//trece la urmatoarea iteratie
}
printf("Conexiune reusita\n");
const char *http_request ="GET / HTTP/1.0\r\nHost: www.yahoo.com\r\n\r\n";//defineste o cerere HTTP de tip GET ca sir constant de caractere
if (send(client_sock, http_request, strlen(http_request), 0) == -1) {//trimite cererea HTTP prin socketul client_socket,verifica daca trimiterea a esuat
printf("Eroare la trimiterea cererii HTTP\n");
close(client_sock);//inchide socket-ul
continue;//trece la urmatoare iteratie
}
printf("Cerere HTTP trimisa\n");
FILE *file = fopen("pagina.html", "w");//deschide un fisier pentru a salva raspunsul
if (file == NULL) {//verifica daca fisierul a fost deschis cu succes
printf("Eroare la deschiderea fisierului pentru salvare\n");
close(client_sock);//inchide socket-ul
continue;//trecere la urmatoarea iteriatie
}
char server_reply[6000];//buffer pentru raspunsul serverului
int received;//declara o variabila,care va stoca numarul de octeti primit prin functia recv
printf("Salvam continutul HTML si afisam in consola...\n");
while ((received = recv(client_sock, server_reply, sizeof(server_reply) -1, 0)) > 0) {//returneaza numarul de octeti primit sau 0 daca conexiunea este inchisa,continua bucla doar daca recv returneaza un numar pozitiv
server_reply[received] ='\0';//pozitioneaza terminatorul de sir la sfarsitul datelor primite,pentru a transforma bufferul intr-un sir de caractere valid

fprintf(file, "%s", server_reply);//scrie continutul in fisierul deschis
printf("%s", server_reply);//afiseaza raspunsul in consola
send(new_fd, server_reply, received, 0);//trimite continutul de la server catre un alt client conectat prin socket
}
fclose(file);//inchide fisierul asociat cu pointerul file
close(client_sock);//inchide conexiune
}
else if (strcmp(buf, "exit#") == 0) {//compara sirul de caractere,returneaza 0 daca sunt identice
printf("Clientul a trimis comanda de iesire\n");
connection_active = 0;//inchide conexiunea curenta
} else {
const char *msg = "Comanda neimplementata";//declara si initializeaza un sir constant cu mesajul
printf("Comanda neimplementata\n");
//trimite mesajul catre clientul conectat prin socket
send(new_fd, msg, strlen(msg), 0);// strlen = functie care determina lungimea unui sir de caractere
} // 0 => nu sunt folosite flaguri speciale
}
}
printf("Conexiunea cu clientul s-a incheiat\n");
close(new_fd);//inchide socket-ul asociat conexiunii clientului
}
close(consocket);//inchide socket principal al serverului,incheind activitatea acestuia
return 0;//programul se incheie cu succes,adica fara erori
}