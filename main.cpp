#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>
#include "bicho.h"
#include "food.h"
#include "obstaculo.h"
#include "parede.h"
#include <cmath>
#include <algorithm>
#include "font.h"
#include "redeneuralbasica.h"
#include "cenario.h"

const int window_width = 800;
const int window_height = 600;
const int NUMERO_OBSTACULOS = 10;
const int NUMERO_COMIDA = 100;
int NUMERO_BICHOS = 20;
int idGeracao = 0;

bool comparaPontuacao(Bicho *b1, Bicho *b2);
void ordenarBichos(std::vector<Bicho*>& bichos);
double distanciaEuclidiana(sf::Vector2f v1, sf::Vector2f v2);
bool dentroDoLimite(sf::Vector2f pos, int x1,int y1, int x2, int y2);
void distribuicaoComida(int quant, std::vector<Food*> &comidas, std::vector<Parede*> &parede, std::vector<Obstaculo*> &obstaculos);


sf::Vector2f posInicial(300.f,300.f);

int main()
{

    srand(static_cast<unsigned int>(time(nullptr)));

    sf::Text txtPainel("painel",FontManager::getInstance().getFont(),20);
    txtPainel.setPosition(sf::Vector2f(400.f,20.f));


    std::vector<std::string> cen = getCenario(1);


    // criação de obstaculos aleatorios na tela
    std::vector<Obstaculo*> obstaculos;
    for(int i = 0 ; i < 60;i++)
    {
        for(int j = 0 ; j < 80 ;j++)
        {
            char c = cen[i][j];
            if(c=='o')
            {
                Obstaculo *o = new Obstaculo(j*10,i*10);
                obstaculos.push_back(o);
            }
        }
    }

    //criar parede
    std::vector<Parede*> parede;
    for(int i = 0 ; i < 60;i++)
    {
        for(int j = 0 ; j < 80 ;j++)
        {
            char c = cen[i][j];
            if(c=='p')
            {
                Parede *p = new Parede(j*10,i*10);
                parede.push_back(p);
            }
        }
    }


    // criação da matriz de comidas aleatorias na tela
    std::vector<Food*> comidas;
    distribuicaoComida(NUMERO_COMIDA,comidas,parede,obstaculos);

    std::vector<Bicho*> bichos;
    for(int i = 0 ; i < NUMERO_BICHOS ;i++)
    {
        Bicho *b = new Bicho();
        b->setId(idGeracao++);
        b->reiniciar(posInicial);
        b->CalculoRedeNeural();
        bichos.push_back(b);
    }
    bichos.at(0)->carregarDadosRedeNeural("bicho0.txt");
    bichos.at(1)->carregarDadosRedeNeural("bicho1.txt");
    bichos.at(2)->carregarDadosRedeNeural("bicho2.txt");
    bichos.at(3)->carregarDadosRedeNeural("bicho3.txt");
    bichos.at(4)->carregarDadosRedeNeural("bicho4.txt");
    bichos.at(5)->carregarDadosRedeNeural("bicho5.txt");
    bichos.at(6)->carregarDadosRedeNeural("bicho6.txt");
    bichos.at(7)->carregarDadosRedeNeural("bicho7.txt");
    bichos.at(8)->carregarDadosRedeNeural("bicho8.txt");
    bichos.at(9)->carregarDadosRedeNeural("bicho9.txt");


    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML window");
    window.setFramerateLimit(60);

    sf::Clock clock;
    sf::Clock clock2;
    float move_speed = 100.f;
    long epoca =0;


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

    clock.restart();
    clock2.restart();

    //comidas = comidasBase;
    distribuicaoComida(NUMERO_COMIDA,comidas,parede, obstaculos);

    std::cout << "Epoca:" << epoca++ << '\n';
    while(clock.getElapsedTime().asSeconds() < 20)
    {
        float deltaTime = clock2.getElapsedTime().asSeconds();
        clock2.restart();

        for(int b = 0 ; b < bichos.size() ;b++)
        {
            //ação para cada bicho do vetor
            Bicho *bicho = bichos.at(b);

            int resultado = bicho->getIdxMaximoSaida();

            if(resultado ==0)
            {
                bicho->giro('D');
            }
            else if(resultado==1)
            {
                bicho->giro('E');
            }

            bicho->move(move_speed * deltaTime);

            bool colisaoParede = bicho->colisaoParede(parede);
            if(colisaoParede==true)
            {
                bicho->setPosition(bicho->getPosicaoAnterior());
                //bicho->somaPontuacao(-10);
                bicho->changeEnergy(-1.0);
            }

            bool colisaoObstaculo = bicho->colisaoObstaculo(obstaculos);
            if(colisaoObstaculo==true)
            {
                bicho->setPosition(bicho->getPosicaoAnterior());
                bicho->somaPontuacao(-10);
                bicho->changeEnergy(-5.0);
            }

            bool colisaoComida = bicho->colisaoComida(comidas);
            if(colisaoComida==true)
            {
                bicho->changeEnergy(5.0);
                bicho->somaPontuacao(10);
            }


            //distancia de visao parede
            double distParedeF=0.0, distParedeD=0.0,distParedeE=0.0;
            for(int p = 0 ; p < parede.size() ;p++)
            {
                int df = bicho->distVisao(parede.at(p)->getShape());
                int dd = bicho->distVisaoDir(parede.at(p)->getShape());
                int de = bicho->distVisaoEsq(parede.at(p)->getShape());
                if(df>distParedeF)distParedeF = df;
                if(dd>distParedeD)distParedeD = dd;
                if(de>distParedeE)distParedeE = de;
            }

            //distancia de visao obstaculo
            double distObstaculoF=0.0, distObstaculoD=0.0,distObstaculoE=0.0;
            for(int o = 0 ; o < obstaculos.size() ;o++)
            {
                int df = bicho->distVisao(obstaculos.at(o)->getShape());
                int dd = bicho->distVisaoDir(obstaculos.at(o)->getShape());
                int de = bicho->distVisaoEsq(obstaculos.at(o)->getShape());
                if(df>distObstaculoF)distObstaculoF = df;
                if(dd>distObstaculoD)distObstaculoD = dd;
                if(de>distObstaculoE)distObstaculoE = de;
            }

            //distancia de visao da comida
            double distComidaF=0.0, distComidaD=0.0, distComidaE=0.0;
            for(int c = 0; c < comidas.size() ;c++)
            {
                int df = bicho->distVisao(comidas.at(c)->getShape());
                int dd = bicho->distVisaoDir(comidas.at(c)->getShape());
                int de = bicho->distVisaoEsq(comidas.at(c)->getShape());
                if(df>distComidaF)distComidaF = df;
                if(dd>distComidaD)distComidaD = dd;
                if(de>distComidaE)distComidaE = de;
            }

            //bicho->imprimeDados();

            //definir valores de entrada
            rn::setValorCamada(bicho->getCamada(0), {distParedeF,distParedeD,distParedeE,distComidaF,distComidaD,distComidaE,distObstaculoF,distObstaculoD,distObstaculoE});
            bicho->CalculoRedeNeural();
         }

        // desenha na janela
        window.clear();

        for(size_t i = 0 ; i < parede.size() ;i++)
        {
            parede.at(i)->draw(window);
        }
        for(size_t i = 0 ; i < obstaculos.size() ;i++)
        {
            obstaculos.at(i)->draw(window);
        }
        for(size_t i = 0; i < comidas.size();i++)
        {
            comidas.at(i)->draw(window);
        }
        for(size_t i = 0 ; i < bichos.size() ;i++)
        {
            bichos.at(i)->draw(window);
        }

        window.display();
        // fim de desenha a janela

    } // while do tempo

    ordenarBichos(bichos);
    //bichos.at(0)->imprimeDados();
    bichos.at(0)->salvarDadosRedeNeural("bicho0.txt");
    bichos.at(1)->salvarDadosRedeNeural("bicho1.txt");
    bichos.at(2)->salvarDadosRedeNeural("bicho2.txt");
    bichos.at(3)->salvarDadosRedeNeural("bicho3.txt");
    bichos.at(4)->salvarDadosRedeNeural("bicho4.txt");
    bichos.at(5)->salvarDadosRedeNeural("bicho5.txt");
    bichos.at(6)->salvarDadosRedeNeural("bicho6.txt");
    bichos.at(7)->salvarDadosRedeNeural("bicho7.txt");
    bichos.at(8)->salvarDadosRedeNeural("bicho8.txt");
    bichos.at(9)->salvarDadosRedeNeural("bicho9.txt");

    int manter = bichos.size() * 0.7;
    while(bichos.size() > manter)
    {
        bichos.pop_back();
        std::cout << "excluindo\n";
    }

    if(bichos.size() > 6)
    {
        int cross = 2;
        for(int b = 0 ; b < 4 ; b+=2)
        {
            Bicho *novo = bichos.at(b)->crossOver(bichos.at(b+1));
            novo->setId(idGeracao++);
            novo->Mutacao();
            bichos.push_back(novo);
            std::cout << "Novo filho \n";
        }
    }

    while(bichos.size() < NUMERO_BICHOS)
    {
        Bicho *b = new Bicho();
        b->setId(idGeracao++);
        bichos.push_back(b);
        std::cout << "Novo individuo\n";
    }


    for (int b = 0; b < bichos.size();b++)
    {
        std::cout << bichos.at(b)->getId() << "-" <<bichos.at(b)->getPontuacao() << '\n';
        bichos.at(b)->reiniciar(posInicial);
    }
    bichos.at(0)->setFillColor(sf::Color::Blue);

    if(NUMERO_BICHOS > 10) NUMERO_BICHOS-=50;

   std::cout << "total de indiviuos=" << bichos.size() << '\n';



    } //while do window

    return 0;
}


bool comparaPontuacao(Bicho *b1, Bicho *b2)
{
    return b1->getPontuacao() > b2->getPontuacao();
}
void ordenarBichos(std::vector<Bicho*>& bichos)
{
    std::sort(bichos.begin(),bichos.end(),comparaPontuacao);
}

double distanciaEuclidiana(sf::Vector2f v1, sf::Vector2f v2)
{
    double x1 = v1.x;
    double x2 = v2.x;
    double y1 = v1.y;
    double y2 = v2.y;
   // std::cout << "\ndistancia euclididana ** = " << std::sqrt( std::pow(x2-x1,2) + std::pow(x2-x1,2) );
    return std::sqrt( std::pow(x2-x1,2) + std::pow(y2-y1,2) );
}

bool dentroDoLimite(sf::Vector2f pos, int x1,int y1, int x2, int y2)
{
    if(pos.x < x1 || pos.x > x2 || pos.y < y1 || pos.y > y2)
    {
        return false;
    }
    return true;
}

void distribuicaoComida(int quant, std::vector<Food*> &comidas, std::vector<Parede*> &parede, std::vector<Obstaculo*> &obstaculos)
{
    int qAtual = comidas.size();
    for(int i = qAtual ; i < quant;i++)
    {
        while(true)
        {
            float x = rand()%(window_width-40)+20;
            float y = rand()%(window_height-40)+20;
            Food *comida = new Food();
            comida->setPosition(x,y);

            int qParede = parede.size();
            int qObstaculos = obstaculos.size();
            int colisao = false;

            for(int j = 0 ; j < qParede;j++)
            {
                if(comida->intersects(parede.at(j)->getShape()))
                {
                    colisao=true;
                }
                if(colisao) break;
            }

            for(int j = 0 ; j < qObstaculos;j++)
            {
                if(comida->intersects(obstaculos.at(j)->getShape()))
                {
                    colisao=true;
                }
                if(colisao) break;
            }

            if(!colisao)
            {
                comidas.push_back(comida);
                break;
            }
        }
    }
}












