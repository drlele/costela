// Árvore 2-3-4  
// (Árvore B em memória, com nós com capacidade para 2, 3 ou 4 filhos)
// Estruturas de Dados - 2018/2
// Código by Joukim

#include <iostream>

using namespace std;

// constantes para verificação
const int MAXITENS = 3;
const int MINITENS = 1;
const int MAXFILHOS = 4;

// posição do elemento do meio (o que sobe para página pai em caso de estouro
const int MEIO = 1; 

typedef float Dado;

class Noh {
friend class Arvore234;
private:
    bool folha;
    int num; // número de itens armazenadas no nó
    Dado itens[MAXITENS];
    Noh* filhos[MAXFILHOS];
    Noh* pai;
public:
    Noh();
    ~Noh();
};

Noh::Noh() {
    num = 0;
    folha = false;
    for (int i = 0; i < MAXFILHOS; i++) {
        filhos[i] = NULL;
    }
}

Noh::~Noh() {
    for (int i = 0; i < num+1; i++) {
        delete filhos[i];
    }
}

class Arvore234 {
private:
    Noh* raiz;
    void percorreEmOrdemAux(Noh* atual, int nivel);
    Noh* divideNoh(Noh* mNoh, Dado &dadoPromovido);
    void InserirFolhaNaoCheia(Noh* mNoh, Dado mDado);
    void InserirNohNaoCheio(Noh* mNoh, Noh* novo, Dado dadoPromovido);
    Noh* inserirAux(Noh* mNoh, Dado mDado, Dado &dadoPromovido);
public:
    Arvore234() { raiz = NULL; }
    ~Arvore234() { delete raiz; }
    // códigos não implementado, sua tarefa é implementá-lo!
    void insere(Dado mDado);
    Dado minimo();
    Dado maximo();
    // percorrimento (impressão)
    void percorreEmOrdem();
};

void Arvore234::insere(Dado mDado) {
    Noh* novoNoh = new Noh;
    if(raiz == NULL) {
        raiz = novoNoh;
        raiz->folha = true;
        raiz->itens[0] = mDado;
        raiz->num = 1;
    }
    else {
        Dado dadoPromovido;
        novoNoh = inserirAux(raiz, mDado, dadoPromovido);
        if(novoNoh != NULL) {
            Noh* antigaRaiz = new Noh;
            Noh* novaRaiz = new Noh;
            antigaRaiz = raiz;
            raiz = novaRaiz;
            raiz->itens[0] = dadoPromovido;
            raiz->num = 1;
            raiz->filhos[0] = antigaRaiz;
            raiz->filhos[1] = novoNoh;
        }
    }
}

Noh* Arvore234::inserirAux(Noh* mNoh, Dado mDado, Dado &dadoPromovido) {
    if(mNoh->folha) {
        if(mNoh->num < MAXITENS) {
            InserirFolhaNaoCheia(mNoh, mDado);
            return NULL;
        }
        else {
            Noh* novoNoh = new Noh;
            novoNoh = divideNoh(mNoh, dadoPromovido);
            if(mDado <= mNoh->itens[MEIO]) {
                InserirFolhaNaoCheia(mNoh, mDado);
            }
            else {
                InserirFolhaNaoCheia(novoNoh, mDado);
            }
            return novoNoh; //dadoPromovido
        }
    }
    else {
        int i = mNoh->num - 1;
        while(i>=0 and mNoh->itens[i]>mDado) {
            i--;
        }
        Noh* nohAux = new Noh;
        nohAux = inserirAux(mNoh->filhos[i+1], mDado, dadoPromovido);
        if(nohAux != NULL) {
            if(mNoh->num < MAXITENS) {
                InserirNohNaoCheio(mNoh, nohAux, dadoPromovido);
                return NULL;
            }
            else {
                Noh* novoNoh = new Noh;
                Dado dadoProvFilho;
                dadoProvFilho = dadoPromovido;
                novoNoh = divideNoh(mNoh, dadoPromovido);
                if(dadoProvFilho <= mNoh->itens[MEIO]) {
                    InserirNohNaoCheio(mNoh, nohAux, dadoProvFilho);
                }
                else {
                    InserirNohNaoCheio(novoNoh, nohAux, dadoProvFilho);
                }
                return novoNoh;
            }
        }
        return NULL;
    }
}

void Arvore234::InserirNohNaoCheio(Noh* mNoh, Noh* novoNoh, Dado dadoPromovido) {
    int pos = mNoh->num - 1;
    while(pos>=0 and mNoh->itens[pos] > dadoPromovido) {
        mNoh->itens[pos+1] = mNoh->itens[pos];
        mNoh->filhos[pos+2] = mNoh->filhos[pos+1];
        pos--;
    }
    mNoh->itens[pos+1] = dadoPromovido;
    mNoh->filhos[pos+2] = novoNoh;

    mNoh->num++;
}

void Arvore234::InserirFolhaNaoCheia(Noh* mNoh, Dado mDado) {
    int pos = mNoh->num - 1;
    while(pos >= 0 and mNoh->itens[pos] > mDado) {
        mNoh->itens[pos+1] = mNoh->itens[pos];
        pos--;
    }
    mNoh->itens[pos+1] = mDado;
    mNoh->num++;
}

Noh* Arvore234::divideNoh(Noh* mNoh, Dado &dadoPromovido) {
    dadoPromovido = mNoh->itens[MEIO];
    Noh* novoNoh = new Noh;
    novoNoh->folha = mNoh->folha;
    novoNoh->itens[0] = mNoh->itens[MEIO+1];

    novoNoh->num = MEIO;
    mNoh->num = MEIO;

    if(!mNoh->folha) {
        for(int i=0;i<(MEIO+1);i++) {
            novoNoh->filhos[i] = mNoh->filhos[(MEIO+1)+i];
        }
    }
    return novoNoh; //dadoPromovido
}

Dado Arvore234::minimo() {
    Noh* atual = raiz;
    while(!atual->folha) {
        atual = atual->filhos[0];
    }
    return atual->itens[0];
}

Dado Arvore234::maximo() {
    Noh* atual = raiz;
    while(!atual->folha) {
        atual = atual->filhos[atual->num];
    }
    return atual->itens[atual->num-1];
}

void Arvore234::percorreEmOrdem() {
    percorreEmOrdemAux(raiz,0);        
    cout<< endl;
}

void Arvore234::percorreEmOrdemAux(Noh* umNoh, int nivel) {
    int i;
    for (i = 0; i < umNoh->num; i++) {
        // se nó não é folha, imprima os dados do filho i
        // antes de imprimir o item i
        if (not umNoh->folha) {
            percorreEmOrdemAux(umNoh->filhos[i], nivel+1); 
        }
        cout << umNoh->itens[i] << '/' << nivel << ' ';
    }
 
    // Imprima os dados do último filho
    if (not umNoh->folha) {
        percorreEmOrdemAux(umNoh->filhos[i], nivel+1);   
    }
}


// =========================== Programa ================================
int main() {
    Arvore234 B;
    
    char opcao;
    float valor;
    
    do {
        cin >> opcao;
        if (opcao == 'i') {
            cin >> valor;
            B.insere(valor);
        } if (opcao == 'l') {
            cout << B.minimo() << endl;
        } if (opcao == 'h') {
            cout << B.maximo() << endl;
        } else if(opcao == 'e') {
            B.percorreEmOrdem();
        }
    } while(opcao != 'q');
    
    return 0;
}
