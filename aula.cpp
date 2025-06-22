#include <iostream>
#define TAM 100

using namespace std;

struct Fila {
    int dados[TAM];
    int inicio, fim;
};

void inicializarFila(Fila* f) {
    f->inicio = 0;
    f->fim = 0;
}

bool filaVazia(Fila* f) {
    return f->inicio == f->fim;
}

bool filaCheia(Fila* f) {
    return f->fim == TAM;
}

bool enfileirar(Fila* f, int valor) {
    if (filaCheia(f)) return false;
    f->dados[f->fim++] = valor;
    return true;
}

bool desenfileirar(Fila* f, int* valor) {
    if (filaVazia(f)) return false;
    *valor = f->dados[f->inicio++];
    return true;
}

int tamanhoFila(Fila* f) {
    return f->fim - f->inicio;
}

void listarFila(Fila* f) {
    for (int i = f->inicio; i < f->fim; i++) {
        cout << f->dados[i] << " ";
    }
    cout << endl;
}

struct Guiche {
    int id;
    Fila* senhasAtendidas;
};

struct NoGuiche {
    Guiche* guiche;
    NoGuiche* prox;
};

NoGuiche* inserirGuiche(NoGuiche* lista, int id) {
    NoGuiche* novo = new NoGuiche;
    novo->guiche = new Guiche;
    novo->guiche->id = id;
    novo->guiche->senhasAtendidas = new Fila;
    inicializarFila(novo->guiche->senhasAtendidas);
    novo->prox = lista;
    cout << "Guiche " << id << " aberto.\n";
    return novo;
}

Guiche* buscarGuiche(NoGuiche* lista, int id) {
    while (lista != nullptr) {
        if (lista->guiche->id == id)
            return lista->guiche;
        lista = lista->prox;
    }
    return nullptr;
}

int contarGuiches(NoGuiche* lista) {
    int count = 0;
    while (lista != nullptr) {
        count++;
        lista = lista->prox;
    }
    return count;
}

int main() {
    Fila senhasGeradas;
    inicializarFila(&senhasGeradas);

    NoGuiche* listaGuiches = nullptr;

    int opcao;
    int senhaAtual = 1;

    do {
        cout << "\n--- Sistema de Atendimento 2.0 ---\n";
        cout << "Senhas aguardando: " << tamanhoFila(&senhasGeradas) << endl;
        cout << "Guiches abertos: " << contarGuiches(listaGuiches) << endl;
        cout << "0 - Sair\n1 - Gerar Senha\n2 - Abrir Guiche\n3 - Realizar Atendimento\n4 - Listar Senhas Atendidas\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 0:
                if (!filaVazia(&senhasGeradas)) {
                    cout << "Ainda ha senhas aguardando. Finalize antes de sair.\n";
                } else {
                    int totalAtendidas = 0;
                    NoGuiche* atual = listaGuiches;
                    while (atual != nullptr) {
                        totalAtendidas += tamanhoFila(atual->guiche->senhasAtendidas);
                        atual = atual->prox;
                    }
                    cout << "\nSistema encerrado.\nTotal de senhas atendidas: " << totalAtendidas << "\n";
                }
                break;

            case 1:
                if (enfileirar(&senhasGeradas, senhaAtual)) {
                    cout << "Senha gerada: " << senhaAtual << endl;
                    senhaAtual++;
                } else {
                    cout << "Erro: fila cheia.\n";
                }
                break;

            case 2: {
                int idGuiche;
                cout << "Digite o ID do guiche a ser aberto: ";
                cin >> idGuiche;

                if (buscarGuiche(listaGuiches, idGuiche)) {
                    cout << "Guiche ja existe.\n";
                } else {
                    listaGuiches = inserirGuiche(listaGuiches, idGuiche);
                }
                break;
            }

            case 3: {
                if (filaVazia(&senhasGeradas)) {
                    cout << "Nenhuma senha aguardando.\n";
                    break;
                }
                int idGuiche;
                cout << "Digite o ID do guiche que realizara o atendimento: ";
                cin >> idGuiche;

                Guiche* g = buscarGuiche(listaGuiches, idGuiche);
                if (g == nullptr) {
                    cout << "Guiche nao encontrado.\n";
                } else {
                    int senha;
                    desenfileirar(&senhasGeradas, &senha);
                    enfileirar(g->senhasAtendidas, senha);
                    cout << "Guiche " << g->id << " atendeu a senha " << senha << endl;
                }
                break;
            }

            case 4: {
                int idGuiche;
                cout << "Digite o ID do guiche para listar senhas atendidas: ";
                cin >> idGuiche;

                Guiche* g = buscarGuiche(listaGuiches, idGuiche);
                if (g == nullptr) {
                    cout << "Guiche nao encontrado.\n";
                } else if (filaVazia(g->senhasAtendidas)) {
                    cout << "Nenhuma senha atendida por este guiche.\n";
                } else {
                    cout << "Senhas atendidas pelo guiche " << idGuiche << ": ";
                    listarFila(g->senhasAtendidas);
                }
                break;
            }

            default:
                cout << "Opcao invalida.\n";
        }

    } while (opcao != 0 || !filaVazia(&senhasGeradas));

    return 0;
}
