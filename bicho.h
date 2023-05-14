#ifndef BICHO_H_INCLUDED
#define BICHO_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "parede.h"
#include "obstaculo.h"
#include "food.h"
#include "redeneuralbasica.h"

//sf::Font font;

class Bicho
{
    float largura = 40.f;
    float altura = 40.f;
    sf::RectangleShape corpo;
    sf::RectangleShape olhoE,olhoD;
    sf::Text *txtIdent;
    sf::Text *txtPontuacao;
    int id = 0;
    char direcao = 'B';
    const double energiaInicial = 1000.0;
    double energia = energiaInicial;
    long pontuacao = 0;
    long caminhada = 0;
    sf::Vector2f posAnterior;
    std::vector<sf::RectangleShape*> visao;
    std::vector<sf::RectangleShape*> visaoDir;
    std::vector<sf::RectangleShape*> visaoEsq;


    double geradorAleatorio(double vMin,double doubleVMax);


    //camada de rede neural
    int nCamadaEntrada = 9;
    int nCamadaO1 = 12;
    int nCamadaO2 = 18;
    int nCamadaSaida = 3;
    std::vector<rn::Neuronio*> camadaEntrada;
    std::vector<rn::Neuronio*> camadaO1;
    std::vector<rn::Neuronio*> camadaO2;
    std::vector<rn::Neuronio*> camadaSaida;

public:

    Bicho();
    ~Bicho();
    Bicho(const Bicho& origem);
    Bicho& operator=(const Bicho& bicho); // Operador de atribuição
    void montarRedeNeural();
    void setPosition(sf::Vector2f posicao);
    sf::Vector2f getPosition();
    float getHeight();
    float getWidth();
    void atualizaVisao();
    // Visualiza objecto no campo de visao do bicho
    int distVisao(sf::Shape &s);
    int distVisaoEsq(sf::Shape &s);
    int distVisaoDir(sf::Shape &s);
    void setDirecao(char direcao);
    char getDirecao() const;
    void move(float m);
    long getCaminhada();
    void draw(sf::RenderWindow &w);
    bool intersects(sf::Shape &shape);
    void setFillColor(sf::Color cor);
    void changeEnergy(double energia);
    bool verifyEnegy(double energia);
    double getValueEnergy();
    void giro(char c);
    sf::Shape& getCorpo();
    void somaPontuacao(long valor);
    long getPontuacao();
    void setPontuacao(long p);
    void setId(int id);
    int getId();



    //Funcao de colisão
    bool colisaoParede(std::vector<Parede*> &parede);
    bool colisaoObstaculo(std::vector<Obstaculo*> &obstaculos);
    bool colisaoComida(std::vector<Food*> &comidas);

    double distanciaEuclidiana(sf::Vector2f v1);

    void setValorCamadaRN(int idCamada, int idNeuronio, double valor);
    double getValorCamadaRN(int idCamada, int idNeuronio) const;

    int getNcamada(int idCamada);

    void CalculoRedeNeural();

    void Mutacao();
    void MutacaoCamada(std::vector<rn::Neuronio*> &camada);

    int getIdxMaximoSaida();

    void setCaminhada(long v);
    void imprimeDados();

    void reiniciar(sf::Vector2f posicao);

    rn::Neuronio* getNeuronio(int idCamada, int idNeuronio);

    Bicho* crossOver(Bicho* gerador2);

    void setPosicaoAnterior(sf::Vector2f pos);
    sf::Vector2f getPosicaoAnterior();

    std::vector<rn::Neuronio*>& getCamada(int idCamada);

    void salvarDadosRedeNeural(std::string nomeArquivo);
    void carregarDadosRedeNeural(std::string nomeArquivo);
};

#endif // BICHO_H_INCLUDED
