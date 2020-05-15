#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100
#define MAXSIGLA 10
#define TAM_NOME 25
//Síntese
//objetivo: registrar cursos profissionalizantes, alterar dados de um curso já cadastrado, listar os cursos cadastrados, consultar modalidades de cursos e remover o arquivo de cadastro
//entrada: para fazer um cadastro, a entrada vai ser: "N",código numérico de identificação do curso, 
//			modalidade do curso, nome completo do curso, sigla da escola.
//			"A" para alterar dados do curso, "L" para listar os cursos, "C" para consultar as modalidades dos cursos cadastrados
//			"R" para remover o arquivo, "E" para encerrar o programa.
//saída: a saída deve ser um novo curso cadastrado, ou os dados de um curso devem ser alterados
//		 ou uma lista dos cursos será mostrada, ou os dados de um curso específico devem ser mostrados,
//		 ou um arquivo deve ser removido
struct infoCurso{
	long int CNIC;
	char nomeCurso[TAM_NOME],modalidade,sigla[MAXSIGLA];
};
//prototipos
void validaNomeCurso(char *pNome);
void validaModalidade(char *pModalidade);
long int validaTamCNIC(long int CNIC);
long int validaCNIC(long int CNIC);
void validaSigla(char *pSigla);
void arrumaNome(char *pNome);
void validaNomeArquivo(char *pNome);
void arrumaSigla(char *pSigla);
void novoCurso(char *opcao,struct infoCurso aluno);
void listaCursos(char *opcao);
void consulta(char *opcao);
void ordenaConsulta(struct infoCurso *aluno, int auxVetor);
void imprimeConsulta(struct infoCurso *aluno,int auxVetor,char modalidade);
void bubbleSort(struct infoCurso *aluno, char escolha,int auxVetor);
void alteraCursos(char *opcao);
void excluiArquivo(char *opcao);
//####### PRINCIPAL ###### 
int main() {
	//declarações
	int linhas;
	char opcao;
	struct infoCurso aluno;
	//implementação
	do{
		for(linhas=0;linhas<110;linhas++){//borda superior
			printf("%c",177);
		}
		printf("\n");
		printf("\t\t\t\t\t\t\t\t CURSOS PROFISSIONALIZANTES\n");
		printf("\t\t\t\tSeja Bem-vindo(a), digite uma das opcoes abaixo\n");
		printf("\t\tN. Novo Curso\n");
		printf("\t\tA. Altera dados do Curso\n");
		printf("\t\tL. Lista todos os Cursos cadastrados\n");
		printf("\t\tC. Consulta os Cursos cadastrados de uma unica modalidade especifica\n");
		printf("\t\tR. Remove arquivo com os cadastros de todos os cursos\n");
		printf("\t\tE. Encerra o programa\n");
		printf("\n");
		for(linhas=0;linhas<110;linhas++){//borda inferior
			printf("%c",177);
		}
		printf("\n");
		fflush(stdin);
		scanf("%c",&opcao);
			switch(opcao){
				case 'n':
				case 'N':
					do{
						novoCurso(&opcao,aluno);
					}while(opcao=='n' || opcao=='N');	
				break;
				case 'a':
				case 'A':
					do{
						alteraCursos(&opcao);
					}while(opcao=='a' || opcao=='A');
				break;
				case 'L':
				case 'l':
					system("cls");
					listaCursos(&opcao);
				break;
				case 'c':
				case 'C':
					system("cls");
					consulta(&opcao);
				break;
				case 'r':
				case 'R':
					system("cls");
					excluiArquivo(&opcao);
				break;
				case 'e':
				case 'E':
				break;
				default:
					system("cls");
					printf("Opcao invalida, digite novamente!\n");
				break;
			}
	}while(opcao!='e' && opcao!='E');
	return 0;
}
//############# SUBPROGRAMAS ########
//objetivo: cadastrar um novo curso
//parâmetros: nenhum
//retorno: nenhum
void novoCurso(char *opcao,struct infoCurso aluno){
	FILE *pArq;
	system("cls");
	printf("Voce escolheu a opcao 'Novo curso', vamos cadastrar\n");
    printf("Digite o nome completo do curso\n");
    fflush(stdin);
	fgets(aluno.nomeCurso,TAM_NOME,stdin);
	validaNomeCurso(aluno.nomeCurso);
	arrumaNome(aluno.nomeCurso);
	system("cls");
	printf("Digite P para cadastrar um curso presencial, e D para cadastrar um curso a distancia\n");
	scanf("%c",&aluno.modalidade);
	fflush(stdin);
	validaModalidade(&aluno.modalidade);
	system("cls");
	printf("Digite o codigo Numerico de Identificacao do Curso (CNIC). O valor deve ser maior que 100\n");
	scanf("%ld",&aluno.CNIC);
	aluno.CNIC=validaTamCNIC(aluno.CNIC);
	aluno.CNIC=validaCNIC(aluno.CNIC);
	system("cls");
	printf("Digite a Sigla da Escola, com o maximo de 10 caracteres\n");
	fflush(stdin);
	fgets(aluno.sigla,MAXSIGLA,stdin);
	fflush(stdin);
	validaSigla(aluno.sigla);
	arrumaSigla(aluno.sigla);
	pArq=fopen("cursos.bin","ab");
	fwrite(&aluno,sizeof(aluno),1,pArq);
	fclose(pArq);
	system("cls");
	printf("Cadastro concluido! para cadastrar mais um curso, digite N, para voltar ao menu, digite M\n");
	printf("Para encerrar o programa, digite 'E'\n");
	scanf("%c",opcao);
	system("cls");
}
//objetivo: alterar dados de algum curso, sobrescrevendo ou excluindo algum item específico, com exceção do CNIC
//parâmetros: opcao do usuario escolheu
//retorno: nenhum
void alteraCursos(char *opcao){
	int aux=0,size=0,posicao=0,tamanho,posicaoBytes=0,achou=0,contador=0,bytes;
	long int cnic;
	struct infoCurso auxArquivo,aluno;
	char tipo,nome[TAM_NOME],sigla[MAXSIGLA],modalidade,escolha,excluir;
	FILE *pTemp;
	FILE *pArq=fopen("cursos.bin","rb");
	fseek(pArq,0,SEEK_END);
	size=ftell(pArq);
	rewind(pArq);
	fclose(pArq);
	tamanho=size/sizeof(auxArquivo);
	struct infoCurso auxiliar;
	do{
		//system("cls");
		printf("Voce escolheu a opcao 'alteracao de dados'\n");
		printf("Digite:\n");
		printf("\tA. Para alterar o cadastro\n");
		printf("\tM. Para voltar ao menu\n");
		fflush(stdin);
		scanf("%c",&escolha);
		switch(escolha){
			case 'a':
			case 'A':
				system("cls");
				printf("Digite o CNIC (codigo numerico de identifacao do curso) do curso que voce quer alterar\n");
				scanf("%ld",&cnic);
				cnic=validaTamCNIC(cnic);
				pArq=fopen("cursos.bin","rb+");
				while(fread(&auxArquivo,sizeof(auxArquivo),1,pArq)&&achou==0){
					if(auxArquivo.CNIC==cnic){
						auxiliar=auxArquivo;
						achou++;
					}
					aux++;
					posicaoBytes=aux*sizeof(auxiliar);
				}
				fclose(pArq);
				if(achou==0){
					system("cls");
					printf("CNIC nao encontrado\n");
				}
				else{
					do{
						printf("Digite:\n");
						printf("N - Para alterar o nome\n");
						printf("S - Para alterar a sigla\n");
						printf("M - Para alterar a modalidade\n");
						printf("C - Para alterar o CNIC\n");
						printf("Observacao: Para alterar o CNIC, o curso sera excluido.\n");
						fflush(stdin);
						scanf("%c",&tipo);
						switch(tipo){
							case 'n':
							case 'N':
									printf("Digite o novo nome do curso de %s\n",auxiliar.nomeCurso);
									fflush(stdin);
									fgets(nome,TAM_NOME,stdin);
									validaNomeCurso(nome);
									arrumaNome(nome);
									strcpy(auxiliar.nomeCurso,nome);
									pArq=fopen("cursos.bin","rb+");
									fseek(pArq,posicaoBytes-sizeof(auxiliar),SEEK_SET);
									fwrite(&auxiliar,sizeof(auxiliar),1,pArq);
									fclose(pArq);
									printf("Alteracao concluida, digite M para voltar ao menu, e A para alterar mais dados\n");
									fflush(stdin);
									scanf("%c",opcao);
						   break;
							case 's':
							case 'S':	
									printf("Digite a nova sigla da escola do curso de %s\n",auxiliar.nomeCurso);
									printf("Sigla atual: %s\n",auxiliar.sigla);
									fflush(stdin);
									fgets(sigla,MAXSIGLA,stdin);
									validaSigla(sigla);
									arrumaSigla(sigla);
									strcpy(auxiliar.sigla,sigla);
									pArq=fopen("cursos.bin","rb+");
									fseek(pArq,posicaoBytes-sizeof(auxiliar),SEEK_SET);
									fwrite(&auxiliar,sizeof(auxiliar),1,pArq);
									fclose(pArq);
									printf("Alteracao concluida, digite M para voltar ao menu, e A para alterar mais dados\n");
									fflush(stdin);
									scanf("%c",opcao);
							break;
							case 'm':
							case 'M':
									printf("Digite a nova modalidade do curso de %s\n",auxiliar.nomeCurso);
									printf("Modalidade atual: %c\n",auxiliar.modalidade);
									printf("P - Para presencial\n");
									printf("D - Para a distancia\n");
									fflush(stdin);
									scanf("%c",&modalidade);
									fflush(stdin);
									validaModalidade(&modalidade);
									auxiliar.modalidade=modalidade;
									pArq=fopen("cursos.bin","rb+");
									fseek(pArq,posicaoBytes-sizeof(auxiliar),SEEK_SET);
									fwrite(&auxiliar,sizeof(auxiliar),1,pArq);
									fclose(pArq);
									printf("Alteracao concluida, digite 'M' para voltar ao menu, e 'A' para alterar mais dados\n");
									scanf("%c",opcao);
							break;
							case 'c':
							case 'C':
								system("cls");
								printf("Voce tem certeza que deseja remover o cadastro com CNIC : %d ?\n",cnic);
								printf("Digite 'S' para confirmar, ou qualquer tecla para cancelar\n");
								fflush(stdin);
								scanf("%c",&excluir);
								if(excluir !='S' && excluir != 's'){
									printf("Operacao cancelada\n");
								}
								else{
									FILE *pTemp;
									pArq=fopen("cursos.bin","rb");
									if(pArq==NULL){
										printf("Nao abriu corretamente\n");
									}
									else{
										printf("cnic: %d\n",cnic);
										while(fread(&auxArquivo,sizeof(auxArquivo),1,pArq)){
											if(auxArquivo.CNIC != cnic){
												pTemp=fopen("temporario.bin","ab");
												fwrite(&auxArquivo,sizeof(auxArquivo),1,pTemp);
												fclose(pTemp);
											}
										}	
										if(fclose(pArq) != 0){
											printf("nao fechou corretamente\n");
										}	
										else{
											if(remove("cursos.bin")==0){
												printf("Removido com sucesso\n");
												rename("temporario.bin","cursos.bin");
											}
											else{
												printf("Erro ao remover\n");
											}
											printf("Digite N para cadastrar um novo curso, e M para voltar ao menu\n");
											fflush(stdin);
											scanf("%c",opcao);
											if(*opcao=='n' || *opcao=='N'){
												novoCurso(opcao,aluno);
											}	
										}	
									}
								}
							break;
							default:
								system("cls");
								printf("Letra invalida, digite novamente\n");
							break;
						}
					}while(tipo != 'n' && tipo != 'N' && tipo != 's' && tipo != 'S' && tipo != 'c' && tipo != 'C' && tipo != 'm' && tipo != 'M');
				}
			break;
			case 'm':
			case 'M':
				*opcao='M';
			break;
			default:
				printf("Letra invalida, digite 'A' caso queira editar dados, e 'M' caso queira voltar ao menu\n");
			break;
				
		}
	}while(escolha!='a' && escolha != 'A' && escolha != 'm' && escolha != 'M');
	//system("cls");
}
//objetivo: listar todos os cursos
//parâmetros: opcao do usuario escolheu, struct com os registros, numero de cadastros feitos
//retorno: nenhum
void listaCursos(char *opcao){
	int linhas,contador,colunas,auxVetor=0,tamanhoArq,bytes;
	struct infoCurso *aluno,auxArquivo;
	char escolha;
	FILE *pArq;
	pArq=fopen("cursos.bin","rb");
	if(fopen==NULL){
		printf("Nao abriu corretamente\n");
	}
	else{
		fseek(pArq,0,SEEK_END);
		tamanhoArq=ftell(pArq);
		bytes=tamanhoArq/sizeof(aluno);
		aluno=(struct infoCurso*)calloc(bytes,sizeof(aluno));
		rewind(pArq);
		while(fread(&auxArquivo,sizeof(auxArquivo),1,pArq)){
			aluno[auxVetor].CNIC=auxArquivo.CNIC;
			strcpy(aluno[auxVetor].nomeCurso,auxArquivo.nomeCurso);
			strcpy(aluno[auxVetor].sigla,auxArquivo.sigla);
			aluno[auxVetor].modalidade=auxArquivo.modalidade;
			auxVetor++;
		}
	}
	if(fclose(pArq)!=0){
		printf("Nao fechou corretamente\n");
	}
	else{
		printf("Digite C para ver a lista em ordem crescente, e D para ordem decrescente\n");
		fflush(stdin);
		scanf("%c",&escolha);
		switch(escolha){
			case 'c':
			case 'C':
				printf("   NOME DO CURSO\t\tCNIC\t\tSIGLA DA ESCOLA\t\t    MODALIDADE\n");
				bubbleSort(aluno,escolha,auxVetor);
				for(contador=0;contador<auxVetor;contador++){
					if(aluno[contador].modalidade=='p'||aluno[contador].modalidade=='P'){
						printf("%d-%s\t\t\t%ld\t\t\t%s\t\t\tPresencial\n",contador+1,aluno[contador].nomeCurso,aluno[contador].CNIC,aluno[contador].sigla);
					}
					else if(aluno[contador].modalidade=='d'||aluno[contador].modalidade=='D'){
						printf("%d-%s\t\t\t%ld\t\t\t%s\t\t\tA Distancia\n",contador+1,aluno[contador].nomeCurso,aluno[contador].CNIC,aluno[contador].sigla);
					}
				}
				printf("Digite 'M' para voltar ao menu, ou 'E' para encerrar\n");
				fflush(stdin);
				scanf("%c",opcao);
				break;
			case'D':
			case'd':
				printf("   NOME DO CURSO\t\tCNIC\t\tSIGLA DA ESCOLA\t\t    MODALIDADE\n");
				bubbleSort(aluno,escolha,auxVetor);
				for(contador=0;contador<auxVetor;contador++){
					if(aluno[contador].modalidade=='p'||aluno[contador].modalidade=='P'){
						printf("%d-%s\t\t\t%ld\t\t\t%s\t\t\tPresencial\n",contador+1,aluno[contador].nomeCurso,aluno[contador].CNIC,aluno[contador].sigla);
					}
					else if(aluno[contador].modalidade=='d'||aluno[contador].modalidade=='D'){
						printf("%d-%s\t\t\t%ld\t\t\t%s\t\t\tA Distancia\n",contador+1,aluno[contador].nomeCurso,aluno[contador].CNIC,aluno[contador].sigla);
					}
				}
				printf("Digite 'M' para voltar ao menu, ou 'E' para encerrar\n");
				fflush(stdin);
				scanf("%c",opcao);
				break;
			default:
				fflush(stdin);
				printf("Escolha invalida\n");
				listaCursos(opcao);
				break;
		}
		free(aluno);
	}
	system("cls");
}
//objetivo: consultar cursos presenciais ou à distância
//parâmetros: opção do usuário
//retorno: nenhum
void consulta(char *opcao){
	char modalidade;
	int auxVetor=0,linhas,colunas,tamanhoArq,bytes;
	struct infoCurso *aluno,auxiliar,auxArquivo;
	FILE *pArq;
	do{
	printf("Voce escolheu a opcao 'Consulta', digite:\n");
	printf("P - Para consultar cursos presenciais\n");
	printf("D - Para consultar cursos a distancia\n");
	fflush(stdin);
	scanf("%c",&modalidade);
	switch(modalidade){
		case 'p':
		case 'P':
			system("cls");
			pArq=fopen("cursos.bin","rb");
			fseek(pArq,0,SEEK_END);
			tamanhoArq=ftell(pArq);
			bytes=tamanhoArq/sizeof(aluno);
			aluno=(struct infoCurso*)calloc(bytes,sizeof(aluno));
			rewind(pArq);
			while(fread(&auxArquivo,sizeof(auxArquivo),1,pArq)){
				if(auxArquivo.modalidade=='p'||auxArquivo.modalidade=='P'){
					aluno[auxVetor].modalidade=auxArquivo.modalidade;
					strcpy(aluno[auxVetor].nomeCurso,auxArquivo.nomeCurso);
					strcpy(aluno[auxVetor].sigla,auxArquivo.sigla);
					aluno[auxVetor].CNIC=auxArquivo.CNIC;
					auxVetor++;
				}
			}
			fclose(pArq);
			ordenaConsulta(aluno,auxVetor);
			imprimeConsulta(aluno,auxVetor,modalidade);
			printf("Digite 'M' para voltar ao menu, ou 'E' para encerrar\n");
			fflush(stdin);
			scanf("%c",opcao);
			break;
		case 'd':
		case 'D':
			system("cls");
			pArq=fopen("cursos.bin","rb");
			fseek(pArq,0,SEEK_END);
			tamanhoArq=ftell(pArq);
			bytes=tamanhoArq/sizeof(aluno);
			aluno=(struct infoCurso*)calloc(bytes,sizeof(aluno));
			rewind(pArq);
			while(fread(&auxArquivo,sizeof(auxArquivo),1,pArq)){
				if(auxArquivo.modalidade=='d'||auxArquivo.modalidade=='D'){
					aluno[auxVetor].modalidade=auxArquivo.modalidade;
					strcpy(aluno[auxVetor].nomeCurso,auxArquivo.nomeCurso);
					strcpy(aluno[auxVetor].sigla,auxArquivo.sigla);
					aluno[auxVetor].CNIC=auxArquivo.CNIC;
					auxVetor++;
				}
			}
			fclose(pArq);
			ordenaConsulta(aluno,auxVetor);
			imprimeConsulta(aluno,auxVetor,modalidade);
			printf("Digite 'M' para voltar ao menu, ou 'E' para encerrar\n");
			fflush(stdin);
			scanf("%c",opcao);
			break;
		default:
			printf("Modalidade invalida, digite P para ensino presencial, e D para a distancia\n");
		}
	}while(modalidade != 'p' && modalidade != 'P' && modalidade != 'd' && modalidade != 'D');
}
//objetivo: excluir o arquivo de registros
//parâmetros: opção do usuário
//retorno: nenhum
void excluiArquivo(char *opcao){
	char escolha;
	FILE * pArq;
	fflush(stdin);
	printf("Voce tem certeza que quer excluir o arquivo? Digite S para confirmar, e N para cancelar\n");
	scanf("%c",&escolha);
		switch(escolha){
			case 'S':
			case 's':
				pArq=fopen("cursos.bin","wb");
				if(fclose(pArq) != 0){
					printf("Ocorreu um erro ao remover o arquivo! Verifique se ele existe\n");
				}
				else if(fclose(pArq)==0){
					printf("Arquivo removido com sucesso!\n");
					printf("Digite 'M' para retornar ao menu, ou 'E' para encerrar");
					scanf("%c",opcao);
				}
			break;
			case 'n':
			case 'N':
				printf("Operacao cancelada!\n");
				printf("Digite 'M' para retornar ao menu, e 'E' para encerrar");
				scanf("%c",opcao);
			break;
		}
}
//objetivo: validar o código numérico de identificação do curso
//parâmetros: código de identificação do curso
//retorno: código de identificação validado
long int validaCNIC(long int CNIC){
	struct infoCurso aluno;
	FILE *pArq;
	pArq=fopen("cursos.bin","rb");
	if(pArq==NULL){
		pArq=fopen("cursos.bin","wb");
	}
	else{
		while(fread(&aluno.CNIC,sizeof(aluno),1,pArq)){
			while(CNIC==aluno.CNIC){
		  		printf("Esse codigo ja existe, digite outro diferente\n");
		  		scanf("%ld",&CNIC);
		  		CNIC=validaTamCNIC(CNIC);
		  		CNIC=validaCNIC(CNIC);
			}
		}
	}
	fclose(pArq);
	  return CNIC;
}
//objetivo: validar o código numérico de identificação do curso
//parâmetros: código de identificação do curso
//retorno: código de identificação validado
long int validaTamCNIC(long int CNIC){
	while(CNIC<=MAX){
		printf("CNIC invalido, digite um valor maior que %d\n",MAX);
		fflush(stdin);
		scanf("%ld",&CNIC);
	}
	return CNIC;
}
//objetivo: tirar o enter da string
//parâmetros: string
//retorno: nenhum
void tiraEnter(char *string){
	int tam;
	tam=strlen(string);
	if(string[tam-1]=='\n'){
		string[strlen(string)-1]='\0';
	}
}
//objetivo: validar o tamanho do nome do curso
//parâmetros: nome do curso
//retorno: nenhum
void validaNomeCurso(char *pNome){
	tiraEnter(pNome);
	  while(strlen(pNome)<5){
	  	 printf("Tamanho invalido, digite o nome do curso com cinco caracteres, ou mais\n");
	  	 fgets(pNome,TAM_NOME,stdin);
	  	 fflush(stdin);
	  	 tiraEnter(pNome);
	  }
}
//objetivo: validar o nome do arquivo
//parâmetros: nome do arquivo
//retorno: nenhum
void validaNomeArquivo(char *pNome){
	tiraEnter(pNome);
	  while(strlen(pNome)<1){
	  	 printf("Tamanho invalido, digite novamente\n");
	  	 fflush(stdin);
	  	 fgets(pNome,TAM_NOME,stdin);
		 fflush(stdin);
	  	 tiraEnter(pNome);
	  }
}
//objetivo: validar o tamanho da sigla da escola
//parâmetros: sigla da escola
//retorno: nenhum
void validaSigla(char *pSigla){
	tiraEnter(pSigla);
	char auxiliar[MAXSIGLA]="",space=' ';
	strcpy(auxiliar,pSigla);
	  while(strlen(pSigla)<=0){
	  	 printf("Tamanho invalido, digite o nome da sigla da escola, com um caractere, ou mais\n");
	  	 fgets(pSigla,MAXSIGLA,stdin);
	  	 fflush(stdin);
	  	 tiraEnter(pSigla);
	  }
	 while(auxiliar[0]==space){
	  	printf("A sigla nao pode conter espacos, digite novamente\n");
	  	fgets(auxiliar,MAXSIGLA,stdin);
	  	fflush(stdin);
	  	tiraEnter(auxiliar);
	  	strcpy(pSigla,auxiliar);
	}
	
}
//objetivo: validar a modalidade do curso
//parâmetros: modalidade do curso
//retorno: modalidade do curso validada
void validaModalidade(char *pModalidade){
	fflush(stdin);
	while(*pModalidade!='p' && *pModalidade!='d' && *pModalidade!='P' && *pModalidade!='D'){
		printf("Modalidade invalida, digite P para ensino presencial, e D para a distancia\n");
		scanf("%c",pModalidade);
		fflush(stdin);
	}
	
}
//objetivo: fazer o nome do curso ficar com a primeira letra maiuscula
//parâmetros: nome
//retorno: nenhum
void arrumaNome(char *pNome){
	int aux;
	for(aux=0;aux<strlen(pNome);aux++){
		pNome[aux]=tolower(pNome[aux]);
	}
  pNome[0]=toupper(pNome[0]);
}
//objetivo: fazer a sigla ser maiuscula
//parâmetros: sigla
//retorno: nenhum
void arrumaSigla(char *pSigla){
	int aux;
	for(aux=0;aux<strlen(pSigla);aux++){
		pSigla[aux]=toupper(pSigla[aux]);
	}
}
//objetivo: ordenar o vetor de struct em ordem alfabética
//parâmetros: sigla
//retorno: nenhum
void bubbleSort(struct infoCurso *aluno, char escolha,int auxVetor){
	int linhas,colunas;
	struct infoCurso auxiliar;
	if(escolha=='c' || escolha=='C'){
		for(linhas=0;linhas<auxVetor;linhas++){
			for(colunas=0;colunas<auxVetor-1;colunas++){
				if(strcmp(aluno[colunas].nomeCurso,aluno[colunas+1].nomeCurso)>0){
					auxiliar=aluno[colunas];
					aluno[colunas]=aluno[colunas+1];
					aluno[colunas+1]=auxiliar;
				}
			}
		}
	}
	else if(escolha=='d' || escolha=='D'){
		for(linhas=0;linhas<auxVetor;linhas++){
			for(colunas=0;colunas<auxVetor-1;colunas++){
				if(strcmp(aluno[colunas].nomeCurso,aluno[colunas+1].nomeCurso)<0){
					auxiliar=aluno[colunas];
					aluno[colunas]=aluno[colunas+1];
					aluno[colunas+1]=auxiliar;
				}
			}
		}
	}
}
//objetivo: imprimir as listas de cursos presenciais ou a distancia
//parâmetros: estrutura com registros, quantidade de registros, modalidade do curso escolhido
//retorno: nenhum
void imprimeConsulta(struct infoCurso *aluno,int auxVetor,char modalidade){
	int aux;
	if(modalidade=='p' || modalidade=='P'){
		printf("\t\tLista dos cursos presenciais\n");
	}
	else if(modalidade=='d' || modalidade=='D'){
		printf("\t\tLista dos cursos a distancia\n");
	}
	printf("   NOME DO CURSO\t\tCNIC\t\tSIGLA DA ESCOLA\n");
		for(aux=0;aux<auxVetor;aux++){
			printf("%d-%s\t\t\t%ld\t\t\t%s\n",aux+1,aluno[aux].nomeCurso,aluno[aux].CNIC,aluno[aux].sigla);
		}
	
}
//objetivo: ordenar o vetor de struct em ordem alfabética das siglas
//parâmetros: estrutura com 
//retorno: nenhum
void ordenaConsulta(struct infoCurso *aluno, int auxVetor){
	int linhas,colunas;
	struct infoCurso auxiliar;
		for(linhas=0;linhas<auxVetor;linhas++){
			for(colunas=0;colunas<auxVetor-1;colunas++){
				if(strcmp(aluno[colunas].sigla,aluno[colunas+1].sigla)>0){
					auxiliar=aluno[colunas];
					aluno[colunas]=aluno[colunas+1];
					aluno[colunas+1]=auxiliar;
				}
			}
		}
	
}
