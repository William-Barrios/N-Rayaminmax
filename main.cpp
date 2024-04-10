#include <iostream>
#include <algorithm>
#include <thread>
#include <cstdlib>
#include <vector>
#include <mutex>

using namespace std;

class tablero {
public:
    int tamanho;
    char** mat;
    char player1;
    char player2;
    int PoswinIA;
    mutex mutex;
    int posPlayer1;
    int posPlayer2;
    int placesEmpty;
    int winner;
    int alpha;
    int betha;

    tablero(int _tamanho) {
        tamanho = _tamanho;
        winner = 0;
        player1 = 'X';
        player2 = 'O';
        posPlayer1 = 0;
        posPlayer2 = 0;
        alpha = -100000;
        betha = 100000;
        PoswinIA = -10;
        placesEmpty = tamanho * tamanho;
        mat = new char* [tamanho];
        for (int i = 0; i < tamanho; i++) {
            mat[i] = new char[tamanho];
        }
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                mat[i][j] = ' ';
            }
        }

    }
    ~tablero() {
        for (int i = 0; i < tamanho; ++i) {
            delete[] mat[i];
        }
        delete[] mat;
        mat = nullptr;
    }

    void isertar(char player, int x, int y);
    void busqHorizontal();
    void busqVertical();
    void busqDiagonal();
    void imprimirTablero();
    void buscarPosibilidades();
    tablero& operator=(const tablero& otro) {
        if (this != &otro) {
            for (int i = 0; i < tamanho; ++i) {
                delete[] mat[i];
            }
            delete[] mat;

            tamanho = otro.tamanho;
            mat = new char* [tamanho];
            for (int i = 0; i < tamanho; i++) {
                mat[i] = new char[tamanho];
            }
            for (int i = 0; i < tamanho; ++i) {
                for (int j = 0; j < tamanho; ++j) {
                    mat[i][j] = otro.mat[i][j];
                }
            }
            player1 = otro.player1;
            player2 = otro.player2;
            posPlayer1 = otro.posPlayer1;
            posPlayer2 = otro.posPlayer2;
            placesEmpty = otro.placesEmpty;
            winner = otro.winner;
        }
        return *this;
    }


};

void tablero::isertar(char player, int x, int y) {
    this->mat[x][y] = player;
    this->placesEmpty -= 1;
}
void tablero::busqHorizontal() {
    int toP1 = 0;
    int toP2 = 0;
    for (int col = 0; col < tamanho; col++) {
        for (int fil = 0; fil < tamanho; fil++) {
            if (mat[col][fil] == this->player1) {
                toP1++;
            }
            if (mat[col][fil] == this->player2)
                toP2++;
        }
        if (toP1 > 0 && toP2 == 0) {
            if (toP1 == tamanho)
                this->winner = 1;
            //std::lock_guard<std::mutex> lock(mutex);
            mutex.lock();
            this->posPlayer1++;
            mutex.unlock();

        }
        else if (toP2 > 0 && toP1 == 0) {
            if (toP2 == tamanho)
                this->winner = 2;
            //std::lock_guard<std::mutex> lock(mutex);
            mutex.lock();
            this->posPlayer2++;
            mutex.unlock();

        }
        toP1 = 0;
        toP2 = 0;

    }
}
void tablero::busqVertical() {
    int toP1 = 0;
    int toP2 = 0;
    for (int fil = 0; fil < tamanho; fil++) {
        for (int col = 0; col < tamanho; col++) {
            if (mat[col][fil] == this->player1) {
                toP1++;
            }
            if (mat[col][fil] == this->player2)
                toP2++;
        }
        if (toP1 > 0 && toP2 == 0) {
            if (toP1 == tamanho)
                this->winner = 1;
            //std::lock_guard<std::mutex> lock(mutex);
            mutex.lock();
            this->posPlayer1++;
            mutex.unlock();
        }
        else if (toP2 > 0 && toP1 == 0) {
            if (toP2 == tamanho)
                this->winner = 2;
            //std::lock_guard<std::mutex> lock(mutex);
            mutex.lock();
            this->posPlayer2++;
            mutex.unlock();
        }
        toP1 = 0;
        toP2 = 0;
    }
}
void tablero::busqDiagonal() {
    int player1DiagonalPrincipal = 0;
    int player2DiagonalPrincipal = 0;
    int player1DiagonalSecundaria = 0;
    int player2DiagonalSecundaria = 0;

    for (int i = 0; i < tamanho; ++i) {
        if (mat[i][i] == this->player1)
            player1DiagonalPrincipal++;
        else if (mat[i][i] == this->player2)
            player2DiagonalPrincipal++;
        if (mat[i][tamanho - 1 - i] == this->player1)
            player1DiagonalSecundaria++;
        else if (mat[i][tamanho - 1 - i] == this->player2)
            player2DiagonalSecundaria++;
    }
    if (player1DiagonalPrincipal > 0 && player2DiagonalPrincipal == 0) {
        if (player1DiagonalPrincipal == tamanho)
            winner = 1;
        //std::lock_guard<std::mutex> lock(mutex);
        mutex.lock();
        this->posPlayer1++;
        mutex.unlock();

    }
    else if (player2DiagonalPrincipal > 0 && player1DiagonalPrincipal == 0) {
        if (player2DiagonalPrincipal == tamanho)
            winner = 2;
        //std::lock_guard<std::mutex> lock(mutex);
        mutex.lock();
        this->posPlayer2++;
        mutex.unlock();
    }
    if (player1DiagonalSecundaria > 0 && player2DiagonalSecundaria == 0) {
        if (player1DiagonalSecundaria == tamanho)
            winner = 1;
        //std::lock_guard<std::mutex> lock(mutex);
        mutex.lock();
        this->posPlayer1++;
        mutex.unlock();
    }
    else if (player2DiagonalSecundaria > 0 && player1DiagonalSecundaria == 0) {
        if (player2DiagonalSecundaria == tamanho)
            winner = 1;
        //std::lock_guard<std::mutex> lock(mutex);
        mutex.lock();
        this->posPlayer2++;
        mutex.unlock();
    }
}
void tablero::imprimirTablero() {
    cout << "  ";
    for (int i = 0; i < tamanho; ++i) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < tamanho; ++i) {
        cout << i << " ";
        for (int j = 0; j < tamanho; ++j) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}
void tablero::buscarPosibilidades() {
    std::thread threadHorizontal(&tablero::busqHorizontal, this);
    std::thread threadVertical(&tablero::busqVertical, this);
    std::thread threadDiagonales(&tablero::busqDiagonal, this);
    threadHorizontal.join();
    threadVertical.join();
    threadDiagonales.join();
}
struct mmTree {
    vector<vector<tablero*>> tree;
    int player;
    char playerChar;
    mmTree(tablero* root, int _player) {
        vector<tablero*> nivel;
        nivel.push_back(root);
        tree.push_back(nivel);
        if (_player == 1) {
            playerChar = 'X';
        }
        else {
            playerChar = 'O';
        }
    }
    void genTree(int depth) {
        for (int i = 1; i <= depth; i++) {
            int lenLevelant = this->tree[i - 1].size();
            vector<tablero*> nivel;
            for (int j = 0; j < lenLevelant; j++) {
                tablero* father = tree[i - 1][j];
                //father->imprimirTablero();
                for (int filas = 0; filas < father->tamanho; filas++) {
                    for (int colum = 0; colum < father->tamanho; colum++) {
                        if (father->mat[filas][colum] == ' ') {
                            tablero* novo = new tablero(father->tamanho);
                            *novo = *father;
                            novo->mat[filas][colum] = this->playerChar;
                            //novo->imprimirTablero();
                            novo->placesEmpty--;
                            nivel.push_back(novo);
                        }
                    }
                }
            }
            this->tree.push_back(nivel);
            if (this->playerChar == 'X')
                this->playerChar = 'O';
            else {
                this->playerChar = 'X';
            }
        }
    }
    tablero* selecOPMINMAX(int depth, int p) {
        //cout<<"depuracion"<<endl;
        tablero* opt = nullptr;
        if (tree[0][0]->placesEmpty == 1) {
            *tree[0][0] = *tree[1][0];
            depth--;
        }
        for (int i = depth; i > 0; i--) {
            //cout<<"nivel: "<< i<<endl;
            int contar = tree[i - 1][0]->placesEmpty;
            //tree[i-1][0]->imprimirTablero();
            //tree[i-1][0]->imprimirTablero();
            for (int nodesN = 0, sons = 0, fathers = 0; nodesN < tree[i].size(); nodesN++, sons++) {
                if (i == depth) {
                    //tree[i][nodesN]->imprimirTablero();
                    //cout<<fathers<<" padre"<<endl;
                    //tree[i][nodesN]->busqHorizontal();
                    //tree[i][nodesN]->busqVertical();
                    //tree[i][nodesN]->busqDiagonal();
                    tree[i][nodesN]->buscarPosibilidades();
                    if (p == 1) {
                        tree[i][nodesN]->PoswinIA = tree[i][nodesN]->posPlayer2 - tree[i][nodesN]->posPlayer1;
                        //cout<<tree[i][nodesN]->PoswinIA<<endl;
                    }
                    else {
                        tree[i][nodesN]->PoswinIA = tree[i][nodesN]->posPlayer1 - tree[i][nodesN]->posPlayer2;
                        //cout<<tree[i][nodesN]->PoswinIA<<endl;
                    }

                    if (nodesN == 0 || sons == 0) {
                        tree[i - 1][fathers]->PoswinIA = tree[i][nodesN]->PoswinIA;
                    }
                    else if ((i - 1) % 2 == 0) {
                        if (tree[i - 1][fathers]->PoswinIA < tree[i][nodesN]->PoswinIA)
                            tree[i - 1][fathers]->PoswinIA = tree[i][nodesN]->PoswinIA;

                    }
                    else {
                        if (tree[i - 1][fathers]->PoswinIA > tree[i][nodesN]->PoswinIA)
                            tree[i - 1][fathers]->PoswinIA = tree[i][nodesN]->PoswinIA;
                    }
                    if ((sons + 1) == contar) {
                        fathers++;
                        //if(fathers<tree[i-1].size())
                        //tree[i-1][fathers]->imprimirTablero();
                        sons = -1;
                    }
                }
                else {
                    //cout<<fathers<<"padre"<<endl;
                    if (nodesN == 0 || sons == 0) {
                        if (i - 1 == 0) {
                            *tree[0][0] = *tree[i][nodesN];
                            tree[0][0]->PoswinIA = tree[i][nodesN]->PoswinIA;
                            //cout<<tree[i][nodesN]->PoswinIA<<endl;
                        }
                        else {
                            tree[i - 1][fathers]->PoswinIA = tree[i][nodesN]->PoswinIA;
                            //cout<<tree[i][nodesN]->PoswinIA<<endl;
                        }
                    }
                    else if ((i - 1) % 2 == 0) {
                        if (tree[i - 1][fathers]->PoswinIA < tree[i][nodesN]->PoswinIA) {
                            if (i - 1 == 0) {
                                *tree[0][0] = *tree[i][nodesN];
                                tree[0][0]->PoswinIA = tree[i][nodesN]->PoswinIA;
                                //cout<<tree[i][nodesN]->PoswinIA<<endl;
                            }
                            else {
                                tree[i - 1][fathers]->PoswinIA = tree[i][nodesN]->PoswinIA;
                                //out<<tree[i][nodesN]->PoswinIA<<endl;
                            }
                        }

                    }
                    else {
                        if (tree[i - 1][fathers]->PoswinIA > tree[i][nodesN]->PoswinIA)
                            tree[i - 1][fathers]->PoswinIA = tree[i][nodesN]->PoswinIA;
                    }
                    if ((sons + 1) == contar) {
                        fathers++;
                        //if(fathers<tree[i-1].size())
                        //tree[i-1][fathers]->imprimirTablero();
                        sons = -1;
                    }

                }

            }
        }
        //this->tree[0][0]->imprimirTablero();
        return tree[0][0];
    }
    ~mmTree() {
        for (int i = 0; i < tree.size(); ++i) {
            for (int j = 0; j < tree[i].size(); ++j) {
                if (tree[i][j] != nullptr) {
                    delete tree[i][j]->mat;
                    tree[i][j] = nullptr; // Esto es opcional pero puede ser útil para evitar liberar el mismo objeto más de una vez.
                }
            }
        }
    }
};



int main(int argc, const char* argv[]) {
    int n;
    cout << "Ingrese N: ";
    cin >> n; //tamanho del tablero

    int position;
    cout << "Ser jugador 1(1) jugador(2): ";
    cin >> position; //First or second
    int depth;
    cout << "Ingrese profundidad arbol: ";
    cin >> depth; //profundidad del arbol
    tablero N1(n);
    N1.imprimirTablero();
    cout << endl;
    //N2.imprimirTablero();
    //N2.isertar('X', 2, 2);
    //N2.imprimirTablero();
    //N1.isertar('O', 1, 1);
    //N1.imprimirTablero();
    if (position == 1) {
        while (1) {
            int x, y;
            cout << "ingrese coord: ";
            cin >> x >> y;
            N1.isertar('X', x, y);
            //system("cls");
            N1.imprimirTablero();
            N1.buscarPosibilidades();
            //cout << N1.posPlayer1<<endl;
            //cout << N1.posPlayer2<<endl;
            if (N1.winner > 0 || N1.placesEmpty == 0)
                break;
            mmTree Arbolito(&N1, 2);
            if (N1.placesEmpty < depth) {
                Arbolito.genTree(N1.placesEmpty);
                Arbolito.selecOPMINMAX(N1.placesEmpty, 1);
            }
            else {
                Arbolito.genTree(depth);
                Arbolito.selecOPMINMAX(depth, 1);
            }
            N1.imprimirTablero();
            //cout<<endl;
            N1.buscarPosibilidades();
            if (N1.winner > 0 || N1.placesEmpty == 0)
                break;

        }
    }
    else {
        while (1) {
            mmTree Arbolito(&N1, 1);
            if (N1.placesEmpty < depth) {
                Arbolito.genTree(N1.placesEmpty);
                Arbolito.selecOPMINMAX(N1.placesEmpty, 2);
            }
            else {
                Arbolito.genTree(depth);
                Arbolito.selecOPMINMAX(depth, 2);
            }
            //system("cls");
            N1.imprimirTablero();
            //cout<<endl;
            N1.buscarPosibilidades();
            if (N1.winner > 0 || N1.placesEmpty == 0)
                break;
            int x, y;
            cout << "Ingrese Coord: ";
            cin >> x >> y;
            N1.isertar('O', x, y);
            //system("cls");
            N1.imprimirTablero();
            N1.buscarPosibilidades();
            //cout<<endl;
            if (N1.winner > 0 || N1.placesEmpty == 0)
                break;
        }
    }


    return 0;
}
