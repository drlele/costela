#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>

using namespace std;

class Noh; // declaraÃ§Ã£o avanÃ§ada

typedef float TValor; // valor da Ã¡rvore

// DeclaraÃ§Ãµes das classes ===================================================

class Noh {
    friend class AVL;
    friend ostream& operator<<(ostream& saida, Noh* ptNoh);
    private:
        Noh (const TValor& v);
        ~Noh();
        TValor mValor;
        Noh* mPtEsq;
        Noh* mPtDir;
        Noh* mPtPai;
        unsigned int mAltura; // folhas tÃªm altura 1
};

// === Classe Noh ==============================================================================


Noh::Noh(const TValor& v) {
    mPtDir = NULL;
	mPtEsq = NULL;
	mPtPai = NULL;
	mAltura = 0;
	mValor = v;
}

Noh::~Noh() {
    if(mPtDir != NULL) {
        delete mPtDir;
    }
    if(mPtEsq != NULL) {
        delete mPtEsq;
    }
}


// Escreve o conteÃºdo de um nÃ³ no formato [altura:valor].
// Escreve "[]" se o ponteiro recebido for NULL.
ostream& operator<<(ostream& saida, Noh* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mValor << ']';
    return saida;
}

class AVL {
    public:
        AVL();
        ~AVL();
        void EscreverNivelANivel(ostream& saida);
        void Inserir(const TValor& valor);
    private:
        int InformaAltura(Noh* mNoh);
        int atualizaAltura(Noh* mNoh);
        int fatorBalanceamento(Noh* mNoh);
        Noh* ArrumarBalanceamento(Noh* mNoh);
        Noh* rotacaoEsquerda(Noh* mNoh);
        Noh* rotacaoDireita(Noh* mNoh);
        Noh* InserirAux(Noh* mNoh,const TValor& valor);

        Noh* mPtRaiz;
};

// === Classe AVL =================================================================================
AVL::AVL() {
    mPtRaiz = NULL;
}

AVL::~AVL() {
    delete mPtRaiz;
}

// Escreve o conteÃºdo da Ã¡rvore nÃ­vel a nÃ­vel, na saÃ­da de dados informada.
// Usado para conferir se a estrutra da Ã¡rvore estÃ¡ correta.
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<Noh*> filhos;
    Noh* fimDeNivel = new Noh(TValor()); // nÃ³ especial para marcar fim de um nÃ­vel
    filhos.push(mPtRaiz);
    filhos.push(fimDeNivel);
    while (not filhos.empty()) {
        Noh* ptNoh = filhos.front();
        filhos.pop();
        if (ptNoh == fimDeNivel) {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else {
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mPtEsq);
                filhos.push(ptNoh->mPtDir);
            }
        }
    }
    delete fimDeNivel;
}

int AVL::InformaAltura(Noh* mNoh) {
    if(mNoh == NULL) {
        return 0;
    }
    else {
        return mNoh->mAltura;
    }
}

int AVL::atualizaAltura(Noh* mNoh) {
    if (mNoh == NULL){
        return 0;
    }
    int esquerda = atualizaAltura(mNoh->mPtEsq);
    int direita = atualizaAltura(mNoh->mPtDir);
    int maior = 0;

    if (esquerda > direita){
        maior = esquerda;
    } else {
        maior = direita;
    }
    mNoh->mAltura = 1 + maior;
    return 1 + maior;
}

int AVL::fatorBalanceamento(Noh* mNoh) {
    int altAvrEsq = InformaAltura(mNoh->mPtEsq);
    int altArvDir = InformaAltura(mNoh->mPtDir);
    int fatorBal = altAvrEsq - altArvDir;
    return fatorBal;
}

Noh* AVL::rotacaoEsquerda(Noh* mNoh) {
    Noh* aux = mNoh->mPtDir;
    mNoh->mPtDir = aux->mPtEsq;

    if(aux->mPtEsq != NULL) {
        aux->mPtEsq->mPtPai = mNoh;
    }

    aux->mPtPai = mNoh->mPtPai;
    if(mNoh == mPtRaiz) {
        mPtRaiz = aux;
    }
    else if(mNoh == mNoh->mPtPai->mPtEsq){
        mNoh->mPtPai->mPtEsq = aux;
    }
    else {
        mNoh->mPtPai->mPtDir = aux;
    }

    aux->mPtEsq = mNoh;
    mNoh->mPtPai = aux;
    atualizaAltura(aux);
    atualizaAltura(mNoh);
    return aux;
}

Noh* AVL::rotacaoDireita(Noh* mNoh) {
    Noh* aux = mNoh->mPtEsq;
    mNoh->mPtEsq = aux->mPtDir;

    if(aux->mPtDir != NULL) {
        aux->mPtDir->mPtPai = mNoh;
    }

    aux->mPtPai = mNoh->mPtPai;
    
    if(mNoh == mPtRaiz) {
        mPtRaiz = aux;
    }
    else if(mNoh == mNoh->mPtPai->mPtEsq) {
        mNoh->mPtPai->mPtEsq = aux;
    }
    else {
        mNoh->mPtPai->mPtDir = aux;
    }
    aux->mPtDir = mNoh;
    mNoh->mPtPai = aux;
    atualizaAltura(aux);
    atualizaAltura(mNoh);
    return aux;
}

// Insere um valor na Ã¡rvore.
void AVL::Inserir(const TValor& valor) {
    mPtRaiz = InserirAux(mPtRaiz, valor);
}

Noh* AVL::InserirAux(Noh* mNoh, const TValor& valor) {
    if(mNoh == NULL) {
        Noh* novo = new Noh(valor);
        return novo;
    }
    else {
        if(valor < mNoh->mValor) {
            mNoh->mPtEsq = InserirAux(mNoh->mPtEsq, valor);
            mNoh->mPtEsq->mPtPai = mNoh;
        }
        else {
            mNoh->mPtDir = InserirAux(mNoh->mPtDir, valor);
            mNoh->mPtDir->mPtPai = mNoh;
        }
    }
    return ArrumarBalanceamento(mNoh);
}

Noh* AVL::ArrumarBalanceamento(Noh* mNoh) {
    atualizaAltura(mNoh);
    int fatorBal = fatorBalanceamento(mNoh);
    if(fatorBal >= -1 and fatorBal <= 1) {
        return mNoh;
    }
    if(fatorBal > 1 and fatorBalanceamento(mNoh->mPtEsq) >= 0) {
        return rotacaoDireita(mNoh);
    }
    if(fatorBal > 1 and fatorBalanceamento(mNoh->mPtEsq) < 0) {
        mNoh->mPtEsq = rotacaoEsquerda(mNoh->mPtEsq);
        return rotacaoDireita(mNoh);
    }
    if(fatorBal < -1 and fatorBalanceamento(mNoh->mPtDir) <= 0) {
        return rotacaoEsquerda(mNoh);
    }
    if(fatorBal < -1 and fatorBalanceamento(mNoh->mPtDir) > 0) {
        mNoh->mPtDir = rotacaoDireita(mNoh->mPtDir);
        return rotacaoEsquerda(mNoh);
    }
}

// === Programa ===================================================================================
int main() {
    AVL minhaArvore;
    char opcao;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> valor;
                minhaArvore.Inserir(valor);
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                minhaArvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "OpÃ§Ã£o invÃ¡lida\n";
        }
    } while (opcao != 'f');
    return 0;
}
