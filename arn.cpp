#include <iostream>
#include <stdexcept>
#include <queue>
using namespace std;

typedef string Dado;
typedef unsigned TChave;
enum Cor {PRETO, VERMELHO};

class Noh {
    friend class ARN;
    friend ostream& operator<<(std::ostream& saida, Noh* ptNoh);
public:
    Noh(Dado _valor, TChave _chave);
    ~Noh();
private:
    void InverterCor();
    bool ChecaSeEhFilhoAEsquerdaDoPai();
    Dado valor;
    TChave chave;
    int grau;
    Cor cor;
    Noh* pai;
    Noh* esquerda;
    Noh* direita;
};

Noh::Noh(Dado _valor, TChave _chave) {
    valor = _valor;
    chave = _chave;
    grau = 1;
    cor = VERMELHO;
    pai = NULL;
    esquerda = NULL;
    direita = NULL; 
}

Noh::~Noh() {
    if (esquerda != NULL) {
        delete esquerda;
    }
    if (direita != NULL) {
        delete direita;
    }
}

void Noh::InverterCor() {
    if (cor == VERMELHO) {
        cor = PRETO;
    } else {
        cor = VERMELHO;
    }
}

bool Noh::ChecaSeEhFilhoAEsquerdaDoPai() {
    if (pai->esquerda == this) {
        return true;
    }
    return false;
}

class ARN {
public:
    ARN();
    ~ARN();
    void Insere(Dado _valor, TChave _chave);
    void EscreverNivelANivel(std::ostream& saida);
private:
    Noh* raiz;
    int altura;
    void AtualizaAltura();
    int AtualizaAlturaAux(Noh* _noh);
    Noh* BuscaNoh(Dado _valor);
    void ChecarBalanceamentoInsercao(Noh* _noh);
    void RotacaoEsquerda(Noh* _noh);
    void RotacaoDireita(Noh* _noh);
    void RecalcularGraus();
    void RecalcularGrausAux(Noh* _noh);
    void RecalcularGrauDeUmNoh(Noh* _noh);
    void CalcularFator(Noh* _noh);
    Noh* GetTio(Noh* _noh);
};

ARN::ARN() {
    altura = 0;
    raiz = NULL;
}

ARN::~ARN() {
    delete raiz;
}

void ARN::AtualizaAltura() {
    altura = AtualizaAlturaAux(raiz);
}

int ARN::AtualizaAlturaAux(Noh* _noh) {
    if (_noh == NULL){
        return 0;
    }
    int esquerda = AtualizaAlturaAux(_noh->esquerda);
    int direita = AtualizaAlturaAux(_noh->direita);
    int maior = 0;

    if (esquerda > direita){
        maior = esquerda;
    } else {
        maior = direita;
    }
    return 1 + maior;
}

void ARN::Insere(Dado _valor,TChave _chave) {
    Noh* novoNoh = new Noh(_valor, _chave);

    if (raiz == NULL) {
        raiz = novoNoh;
        novoNoh->cor = PRETO;
    } else {
        Noh* percorredor = raiz;
        bool inserido = false;

        while (inserido == false) {
            if (_chave < percorredor->chave) {
                if (percorredor->esquerda == NULL) {
                    percorredor->esquerda = novoNoh;
                    inserido = true;
                } else {
                    percorredor = percorredor->esquerda;
                }
            } else {
                if (percorredor->direita == NULL) {
                    percorredor->direita = novoNoh;
                    inserido = true;
                } else {
                    percorredor = percorredor->direita;
                }
            }
        } //fim do while
        novoNoh->pai = percorredor;
        novoNoh->grau = percorredor->grau + 1;
    }
    if (novoNoh != raiz and novoNoh->pai->cor == VERMELHO) {
        ChecarBalanceamentoInsercao(novoNoh);
    }
    AtualizaAltura();
    RecalcularGraus();
}

void ARN::ChecarBalanceamentoInsercao(Noh* _noh) {
    Noh* tio = GetTio(_noh);
    Noh* avo = _noh->pai->pai;
    Noh* pai = _noh->pai;

    //CASO 1: pai VERMELHO, avo PRETO e tio VERMELHO
    //SOLUÇÃO: trocar as cores de pai, avo e tio
    if (pai->cor == VERMELHO and (tio != NULL and tio->cor == VERMELHO)) {
        pai->InverterCor();
        avo->InverterCor();
        tio->InverterCor();
    //CASO 2: pai VERMELHO, avo PRETO e tio PRETO
    //SOLUÇÃO: rotações
    } else if (pai->cor == VERMELHO and (tio == NULL or tio->cor == PRETO)) {
        //semelhante a fator 2 e fator do filho 0 ou 1 - rotação simples
        if (_noh->ChecaSeEhFilhoAEsquerdaDoPai() and pai->ChecaSeEhFilhoAEsquerdaDoPai()) {
            RotacaoDireita(avo);
            pai->InverterCor();
        } //semelhante a fator -2 e fator do filho 0 ou -1 - rotação simples
        else if (!_noh->ChecaSeEhFilhoAEsquerdaDoPai() and !pai->ChecaSeEhFilhoAEsquerdaDoPai()) {
            RotacaoEsquerda(avo);
            pai->InverterCor();
        } //semelhante a fator 2 e fator do filho -1 - rotaçaõ dupla
        else if (!_noh->ChecaSeEhFilhoAEsquerdaDoPai() and pai->ChecaSeEhFilhoAEsquerdaDoPai()) {
            RotacaoEsquerda(pai);
            RotacaoDireita(avo);
            _noh->InverterCor();
        } // semelhante a fator -2 e fator do filho 1
        else if (_noh->ChecaSeEhFilhoAEsquerdaDoPai() and !pai->ChecaSeEhFilhoAEsquerdaDoPai()) {
            RotacaoDireita(pai);
            RotacaoEsquerda(avo);
            _noh->InverterCor();
        }
        //troca a cor do avo indenpendentemente
        avo->InverterCor();
    }
    //forçar a raiz a ser preta
    if (raiz->cor == VERMELHO) {
        raiz->InverterCor();
    }
}

void ARN::RotacaoEsquerda(Noh* _noh) {
    //salvar quem era o filho a direita do _noh
    Noh* antigoFilhoADireita = _noh->direita;
    //_noh ganha novo filho a direita, o filho a esquerda do antigoFilhoADireita
    _noh->direita = antigoFilhoADireita->esquerda;
    //se esse filho a esquerda existir, ele reconhece a paternidade
    if (antigoFilhoADireita->esquerda != NULL) {
        antigoFilhoADireita->esquerda->pai = _noh;
    }
    //o pai do _noh se torna pai do antigoFilhoADireita
    if (_noh->pai != NULL) {
        if (_noh->pai->esquerda == _noh) {
            _noh->pai->esquerda = antigoFilhoADireita;
        } else {
            _noh->pai->direita = antigoFilhoADireita;
        }
    } else {//pai do noh eh a raiz, entao antigoFilhoADireita eha  nova raiz
        raiz = antigoFilhoADireita;
    }
    //antigoFilhoADireita reconhece o novo pai
    antigoFilhoADireita->pai = _noh->pai;
    //o pai do _noh agora vai ser o antigoFilhoADireita
    _noh->pai = antigoFilhoADireita;
    //antigoFilhoADireita reconhece _noh como seu filho
    antigoFilhoADireita->esquerda = _noh;
}

void ARN::RotacaoDireita(Noh* _noh) {
    //salvar quem era o filho a esquerda do _noh
    Noh* antigoFilhoAEsquerda = _noh->esquerda;
    //_noh ganha novo filho a esquerda, o filho a direita do antigoFilhoAEsquerda
    _noh->esquerda = antigoFilhoAEsquerda->direita;
    //se esse filho a direita existir, ele reconhece a nova paternidade
    if (antigoFilhoAEsquerda->direita != NULL) {
        antigoFilhoAEsquerda->direita->pai = _noh;
    }
    //o pai do _noh se torna pai do antigoFilhoADireita
    if (_noh->pai != NULL) {
        if (_noh->pai->esquerda == _noh) {
            _noh->pai->esquerda = antigoFilhoAEsquerda;
        } else {
            _noh->pai->direita = antigoFilhoAEsquerda;
        }
    } else {//pai do noh eh a raiz, entao antigoFilhoAEsquerda eha  nova raiz
        raiz = antigoFilhoAEsquerda;
    }
    //antigoFilhoAEsquerda reconhece o novo pai
    antigoFilhoAEsquerda->pai = _noh->pai;
    //o pai do _noh agora vai ser o antigoFilhoADireita
    _noh->pai = antigoFilhoAEsquerda;
    //antigoFilhoADireita reconhece _noh como seu filho
    antigoFilhoAEsquerda->direita = _noh;
}

void ARN::RecalcularGraus() {
    RecalcularGrausAux(raiz);
}

void ARN::RecalcularGrausAux(Noh* _noh) {
    if (_noh != NULL) {
        if (_noh->esquerda == NULL and _noh->direita == NULL) {
            RecalcularGrauDeUmNoh(_noh);
        } else {
            RecalcularGrausAux(_noh->esquerda);
            RecalcularGrausAux(_noh->direita);
        }
    }
}

void ARN::RecalcularGrauDeUmNoh(Noh* _noh) {
    if (_noh != NULL) {
        _noh->grau = 1;
        Noh* percorredor = _noh->pai;

        while (percorredor != NULL) {
            _noh->grau++;
            percorredor = percorredor->pai;
        }
        if (_noh->pai != NULL) {
            RecalcularGrauDeUmNoh(_noh->pai);
        }
    }
}

Noh* ARN::GetTio(Noh* _noh) {
    if (_noh->pai != NULL and _noh->pai != raiz) {
        //checa se o pai é filho a direita ou a esquerda do avo
        if (_noh->pai->ChecaSeEhFilhoAEsquerdaDoPai()) {
            return _noh->pai->pai->direita;
        } else {
            return _noh->pai->pai->esquerda;
        }
    }
    return NULL;
}

void ARN::EscreverNivelANivel(ostream& saida) {
    queue<Noh*> filhos;
    filhos.push(raiz);
    while (not filhos.empty()) {
        unsigned nroNohsNesteNivel = unsigned(filhos.size());
        for (unsigned i = 0; i < nroNohsNesteNivel; ++i) {
            Noh* ptNoh = filhos.front();
            filhos.pop();
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->esquerda);
                filhos.push(ptNoh->direita);
            }
        }
        cout << "\n";
    }
}

ostream& operator<<(ostream& saida, Noh* ptNoh) {
    static char vetLetrasCores[] = "PV";
    if (ptNoh == NULL)
        saida << "NIL";
    else
        saida << '[' << vetLetrasCores[ptNoh->cor] << ':' << ptNoh->chave << '/'
              << ptNoh->valor << ']';
    return saida;
}

int main() {
    ARN arvore;
    char opcao;
    Dado valor;
    TChave chave;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                arvore.Insere(valor, chave);
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                arvore.EscreverNivelANivel(cout);
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
