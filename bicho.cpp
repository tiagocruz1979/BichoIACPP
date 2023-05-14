#include "bicho.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include "font.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

    Bicho::Bicho()
    {
        corpo.setSize(sf::Vector2f(largura,altura));
        corpo.setFillColor(sf::Color::Yellow);
        olhoE.setSize(sf::Vector2f(8.f,8.f));
        olhoE.setFillColor(sf::Color::Black);
        olhoD.setSize(sf::Vector2f(8.f,8.f));
        olhoD.setFillColor(sf::Color::Black);
        montarRedeNeural();
        for(int i = 0 ; i < 5 ;i++) // visao frontal
        {
            sf::RectangleShape *vista = new sf::RectangleShape();
            vista->setPosition(getPosition());
            vista->setSize(sf::Vector2f(this->largura,this->altura));
            vista->setFillColor(sf::Color(100,100,100,60));
            visao.push_back(vista);
        }
        for(int i = 0 ; i < 3 ;i++) // visao Esquerda
        {
            sf::RectangleShape *vista = new sf::RectangleShape();
            vista->setPosition(getPosition());
            vista->setSize(sf::Vector2f(this->largura,this->altura));
            vista->setFillColor(sf::Color(100,100,100,60));
            visaoEsq.push_back(vista);
        }
        for(int i = 0 ; i < 3 ;i++) // visao Direita
        {
            sf::RectangleShape *vista = new sf::RectangleShape();
            vista->setPosition(getPosition());
            vista->setSize(sf::Vector2f(this->largura,this->altura));
            vista->setFillColor(sf::Color(100,100,100,60));
            visaoDir.push_back(vista);
        }


        //text.setFont(FontManager::getInstance().getFont());

        this->txtIdent = new sf::Text(std::to_string(this->id),FontManager::getInstance().getFont(),20);
        this->txtIdent->setFillColor(sf::Color::Black);

        this->txtPontuacao = new sf::Text(std::to_string(this->pontuacao),FontManager::getInstance().getFont(),12);
        this->txtPontuacao->setFillColor(sf::Color::White);
    }



    //construtor de Cópias
     Bicho::Bicho(const Bicho& b){
        largura = b.largura;
        altura = b.altura;
        corpo = b.corpo;
        olhoE = b.olhoE;
        olhoD = b.olhoD;
        direcao = b.direcao;
        energia = b.energia;
        pontuacao = b.pontuacao;
        caminhada = b.caminhada;
        posAnterior = b.posAnterior;
        visao = b.visao;
        visaoDir = b.visaoDir;
        visaoEsq = b.visaoEsq;

        // Copia os neuronios da camada de Entrada
        for (int i = 0; i < b.nCamadaEntrada; i++){
            rn::Neuronio* n = new rn::Neuronio(*b.camadaEntrada[i]);
            camadaEntrada.push_back(n);
        }

        // Copia os neuronios da camada Oculta 1
        for (int i = 0; i < b.nCamadaO1; i++){
            rn::Neuronio* n = new rn::Neuronio(*b.camadaO1[i]);
            camadaO1.push_back(n);
        }

        // Copia os neuronios da camada Oculta 2
        for (int i = 0; i < b.nCamadaO2; i++){
            rn::Neuronio* n = new rn::Neuronio(*b.camadaO2[i]);
            camadaO2.push_back(n);
        }

        // Copia os neuronios da camada 2
        for (int i = 0; i < b.nCamadaSaida; i++){
            rn::Neuronio* n = new rn::Neuronio(*b.camadaSaida[i]);
            camadaSaida.push_back(n);
        }
    }

    Bicho& Bicho::operator=(const Bicho& bicho)
    {

    }

    Bicho::~Bicho()
    {
            delete txtIdent;
            delete txtPontuacao;
            for (auto n : camadaEntrada) {
                delete n;
            }
            for (auto n : camadaO1) {
                delete n;
            }
            for (auto n : camadaO2) {
                delete n;
            }
            for (auto n : camadaSaida) {
                delete n;
            }
    }


    void Bicho::setPosition(sf::Vector2f posicao)
    {
        corpo.setPosition(posicao);
        sf::Vector2f posOlhoE,posOlhoD;
        if(this->direcao=='D')
        {
            posOlhoE = sf::Vector2f(posicao.x+largura-8.f,posicao.y+8.f);
            posOlhoD = sf::Vector2f(posicao.x+largura-8.f,posicao.y+24.f);
        }
        else if (this->direcao=='E')
        {
            posOlhoE = sf::Vector2f(posicao.x,posicao.y+8.f);
            posOlhoD = sf::Vector2f(posicao.x,posicao.y+24.f);
        }
        else if (this->direcao=='C')
        {
            posOlhoE = sf::Vector2f(posicao.x+8.f,posicao.y);
            posOlhoD = sf::Vector2f(posicao.x+24.f,posicao.y);
        }
        else // direcao == 'B'
        {
            posOlhoE = sf::Vector2f(posicao.x+8.f,posicao.y+this->altura-8.f);
            posOlhoD = sf::Vector2f(posicao.x+24.f,posicao.y+this->altura-8.f);
        }

        olhoE.setPosition(posOlhoE);
        olhoD.setPosition(posOlhoD);
        atualizaVisao();
    }



    sf::Vector2f Bicho::getPosition()
    {
        return this->corpo.getPosition();
    }



    float Bicho::getHeight(){return this->altura;}



    float Bicho::getWidth(){return this->largura;}



    void Bicho::atualizaVisao()
    {
        float esp = 40.f;
        for(size_t i = 0 ; i < this->visao.size() ;i++)
        {
            float x = this->corpo.getPosition().x;
            float y = this->corpo.getPosition().y;
            sf::RectangleShape *vista = visao.at(i);

            if(this->direcao=='E')          x = x - (i*esp+esp);
            else if(this->direcao=='D')      x = x + (i*esp+esp);
            else if (this->direcao=='C')    y = y - (i*esp+esp);
            else if(this->direcao=='B')     y = y + (i*esp+esp);

            vista->setPosition(sf::Vector2f(x,y));
        }

        for(size_t i = 0 ; i < this->visaoEsq.size() ;i++)
        {
            float x = this->corpo.getPosition().x;
            float y = this->corpo.getPosition().y;
            sf::RectangleShape *vista = visaoEsq.at(i);

            if(this->direcao=='E')          y = y - (i*esp+esp);
            else if(this->direcao=='D')      y = y + (i*esp+esp);
            else if (this->direcao=='C')    x = x + (i*esp+esp);
            else if(this->direcao=='B')     x = x - (i*esp+esp);

            vista->setPosition(sf::Vector2f(x,y));
        }

        for(size_t i = 0 ; i < this->visaoDir.size() ;i++)
        {
            float x = this->corpo.getPosition().x;
            float y = this->corpo.getPosition().y;
            sf::RectangleShape *vista = visaoDir.at(i);

            if(this->direcao=='E')          y = y + (i*esp+esp);
            else if(this->direcao=='D')      y = y - (i*esp+esp);
            else if (this->direcao=='C')    x = x - (i*esp+esp);
            else if(this->direcao=='B')     x = x + (i*esp+esp);

            vista->setPosition(sf::Vector2f(x,y));
        }
     }



     // Visualiza objecto no campo de visao do bicho
     int Bicho::distVisao(sf::Shape &s)
     {
         size_t tam = this->visao.size() - 1;
         for(int i = tam ; i >= 0 ;i--)
         {
             if(visao.at(i)->getGlobalBounds().intersects(s.getGlobalBounds()))
             {
                 return tam - i + 1;
             }
         }
         return 0;
     }



     int Bicho::distVisaoEsq(sf::Shape &s)
     {
         size_t tam = this->visaoEsq.size() - 1;
         for(int i = tam ; i >= 0 ;i--)
         {
             if(visaoEsq.at(i)->getGlobalBounds().intersects(s.getGlobalBounds()))
             {
                 return tam - i + 1;
             }
         }
         return 0;
     }



     int Bicho::distVisaoDir(sf::Shape &s)
     {
         size_t tam = this->visaoDir.size() - 1;
         for(int i = tam ; i >= 0 ;i--)
         {
             if(visaoDir.at(i)->getGlobalBounds().intersects(s.getGlobalBounds()))
             {
                 return tam - i + 1;
             }
         }
         return 0;
     }



    void Bicho::setDirecao(char direcao)
    {
        this->direcao=direcao;
    }



    void Bicho::move(float m)
    {
        if(!verifyEnegy(1.0f))
        {
            setPosition(posAnterior);
            return;
        }

        char d = this->direcao;

        this->posAnterior = getPosition();

        sf::Vector2f pos = getPosition();

        if(d=='D')
        {
            setPosition(sf::Vector2f(pos.x+m,pos.y));
        }
        else if(d=='E')
        {
            setPosition(sf::Vector2f(pos.x-m,pos.y));
        }
        else if(d=='C')
        {
            setPosition(sf::Vector2f(pos.x,pos.y-m));
        }
        else
        {
            setPosition(sf::Vector2f(pos.x,pos.y+m));
        }
        changeEnergy(-1.0);
        atualizaVisao();
    }



    long Bicho::getCaminhada(){return this->caminhada;}



    void Bicho::draw(sf::RenderWindow &w)
    {
        w.draw(this->corpo);
        w.draw(this->olhoE);
        w.draw(this->olhoD);

        this->txtIdent->setPosition(this->getPosition());
        this->txtIdent->move(10.f,10.f);
        w.draw(*(this->txtIdent));

        this->txtPontuacao->setPosition(this->getPosition());
        this->txtPontuacao->move(-20.f,-30.f);
        this->txtPontuacao->setString(std::to_string(getPontuacao()));
        w.draw(*(this->txtPontuacao));


        for(int i = 0 ; i < visao.size() ;i++)
        {
            sf::RectangleShape r;
            r = *(this->visao.at(i));
            w.draw(r);
        }
        for(int i = 0 ; i < visaoEsq.size() ;i++)
        {
            sf::RectangleShape r;
            r = *(this->visaoEsq.at(i));
            w.draw(r);
        }
        for(int i = 0 ; i < visaoDir.size() ;i++)
        {
            sf::RectangleShape r;
            r = *(this->visaoDir.at(i));
            w.draw(r);
        }
    }



    bool Bicho::intersects(sf::Shape &shape)
    {
        return this->corpo.getGlobalBounds().intersects(shape.getGlobalBounds());
    }



    void Bicho::setFillColor(sf::Color cor)
    {
        corpo.setFillColor(cor);
    }



    void Bicho::changeEnergy(double energia)
    {
        this->energia+=energia;
    }



    bool Bicho::verifyEnegy(double energia)
    {
        if(this->energia>=energia)
        {
            return true;
        }
        else
        {
            setFillColor(sf::Color::Red);
        }
    }



    double Bicho::getValueEnergy(){
        return this->energia;
    }



    char Bicho::getDirecao() const {return this->direcao;}



    sf::Shape& Bicho::getCorpo()
    {
        return this->corpo;
    }

    void Bicho::giro(char c)
    {

        if(!verifyEnegy(1.0f))
        {
            return;
        }

        char dir = getDirecao();
        if(c=='D')
        {
            if(dir=='D') setDirecao('B');
            else if(dir=='E') setDirecao('C');
            else if(dir=='C') setDirecao('D');
            else setDirecao('E');
        }
        else
        {
            if(dir=='D') setDirecao('C');
            else if(dir=='E') setDirecao('B');
            else if(dir=='C') setDirecao('E');
            else setDirecao('D');
        }
    }



    void Bicho::montarRedeNeural()
    {
        //montando a rede neural

        //Camada de entrada
        this->camadaEntrada = rn::criarCamadaEntrada(this->nCamadaEntrada,"none");

        //camada oculta 1
        this->camadaO1 = rn::criarCamada(this->nCamadaO1,"relu",camadaEntrada);

        for(int i = 0 ; i < this->nCamadaO1 ;i++) // insere bias e pesos aleatorios em todos os neuronios da camada
        {
            double bias = geradorAleatorio(-1.0,1.0);
            camadaO1.at(i)->setBias(bias);

            for(int j = 0 ; j < this->nCamadaEntrada;j++)
            {
                double peso = geradorAleatorio(-1.0,1.0);
                camadaO1.at(i)->setPesoEntrada(j,peso);
            }
        }

        //Camada oculta 2
        this->camadaO2 = rn::criarCamada(this->nCamadaO2,"relu",camadaO1);

        for(int i = 0 ; i < this->nCamadaO2 ;i++) // insere bias e pesos aleatorios em todos os neuronios da camada
        {
            double bias = geradorAleatorio(-1.0,1.0);
            camadaO2.at(i)->setBias(bias);

            for(int j = 0 ; j < this->nCamadaO1;j++)
            {
                double peso = geradorAleatorio(-1.0,1.0);
                camadaO2.at(i)->setPesoEntrada(j,peso);
            }
        }

        //camada Saida
        this->camadaSaida = rn::criarCamada(this->nCamadaSaida,"none",camadaO2);

        for(int i = 0 ; i < this->nCamadaSaida ;i++) // insere bias e pesos aleatorios em todos os neuronios da camada
        {
            double bias = geradorAleatorio(-1.0,1.0);
            camadaSaida.at(i)->setBias(bias);

            for(int j = 0 ; j < this->nCamadaO2;j++)
            {
                double peso = geradorAleatorio(-1.0,1.0);
                camadaSaida.at(i)->setPesoEntrada(j,peso);
            }
        }
    }



    double Bicho::geradorAleatorio(double vMin,double vMax)
    {
        return (double)std::rand() / RAND_MAX *(vMax-vMin) + vMin;
    }



    bool Bicho::colisaoParede(std::vector<Parede*> &parede)
    {
        for(size_t i = 0 ; i < parede.size();i++)
        {
            if( this->corpo.getGlobalBounds().intersects( parede.at(i)->getShape().getGlobalBounds() ))
            {
                return true;
            }
    }
        return false;
    }



    bool Bicho::colisaoObstaculo(std::vector<Obstaculo*> &obstaculos)
    {
        for(size_t i = 0 ; i < obstaculos.size();i++)
        {
            if( this->corpo.getGlobalBounds().intersects( obstaculos.at(i)->getShape().getGlobalBounds() ))
        {
            return true;
        }
    }
    return false;
    }



    bool Bicho::colisaoComida(std::vector<Food*> &comidas)
    {
        for(size_t i = 0 ; i < comidas.size();i++)
        {
            if( this->corpo.getGlobalBounds().intersects( comidas.at(i)->getShape().getGlobalBounds() ))
                {
                    comidas.erase(comidas.begin()+i);
                    return true;
                }
        }
    return false;
    }



    double Bicho::distanciaEuclidiana(sf::Vector2f v1)
    {
        double x1 = v1.x;
        double x2 = getPosition().x;
        double y1 = v1.y;
        double y2 = getPosition().y;
        return std::sqrt( pow(x1-x2,2) + pow(y1-y2,2) );
    }



    void Bicho::setValorCamadaRN(int idCamada, int idNeuronio, double valor)
    {
        if(idCamada==0)
        {
            this->camadaEntrada.at(idNeuronio)->setValor(valor);
        }
        else if(idCamada==1)
        {
            this->camadaO1.at(idNeuronio)->setValor(valor);
        }
        else if(idCamada==2)
        {
            this->camadaO2.at(idNeuronio)->setValor(valor);
        }
        else if(idCamada==3)
        {
            this->camadaSaida.at(idNeuronio)->setValor(valor);
        }
    }



    double Bicho::getValorCamadaRN(int idCamada, int idNeuronio) const
    {
        if(idCamada==0)
        {
            return this->camadaEntrada.at(idNeuronio)->getValor();
        }
        else if(idCamada==1)
        {
            return this->camadaO1.at(idNeuronio)->getValor();
        }
        else if(idCamada==2)
        {
            return this->camadaO2.at(idNeuronio)->getValor();
        }
        else if(idCamada==3)
        {
            return this->camadaSaida.at(idNeuronio)->getValor();
        }
        std::cout << "impossivel recuperar valor de bicho::getValorCamadaRN\n";
        return 0.0;
    }



    int Bicho::getNcamada(int idCamada)
    {
        if(idCamada==0) return this->nCamadaEntrada;
        else if (idCamada==1) return this->nCamadaO1;
        else if(idCamada==2) return this->nCamadaO2;
        else if(idCamada==3) return this->nCamadaSaida;
        else return 0;
    }



    void Bicho::CalculoRedeNeural()
    {
        rn::calcularSaidaCamada(getCamada(1));
        rn::calcularSaidaCamada(getCamada(2));
        rn::calcularSaidaCamada(getCamada(3));

    }

    void Bicho::Mutacao()
    {
        MutacaoCamada(this->camadaO1);
        MutacaoCamada(this->camadaO2);
        MutacaoCamada(this->camadaSaida);
    }

    void Bicho::MutacaoCamada(std::vector<rn::Neuronio*> &camada)
    {
        for( size_t i = 0 ; i < camada.size() ;i++)
        {
            double bias = camada.at(i)->getBias();
            double mutacao = geradorAleatorio(0.98,1.02);
            double aleatorio = geradorAleatorio(0.0,10.0);
            if(aleatorio<=3.0)
            {
               // std::cout << "\nbias alterado" << bias*mutacao;
                camada.at(i)->setBias(bias*mutacao);
            }

                for(size_t j = 0 ; j < camada.at(i)->getQEntradas();j++)
                {
                    double peso = camada.at(i)->getPesoEntrada(j);
                    double mutacao = geradorAleatorio(0.98,1.02);
                    double aleatorio = geradorAleatorio(0.0,10.0);
                    if(aleatorio<=3.0)
                    {
                       // std::cout << "\npeso alterado" << peso*mutacao;
                        camada.at(i)->setPesoEntrada(j,peso*mutacao);
                    }
                }
        }
    }


    int Bicho::getIdxMaximoSaida()
    {
        double maximo = 0.0;
        int idx = -1;
        for(size_t i = 0; i < this->camadaSaida.size();i++)
        {
            //std::cout << "\nSaida Camada " << i << " " << camada2.at(i)->getValor();
            if(i==0)
            {
                maximo = camadaSaida.at(i)->getValor();
                idx = i;
            }
            else if(camadaSaida.at(i)->getValor() > maximo)
            {
                maximo = camadaSaida.at(i)->getValor();
                idx = i;
            }
        }
        return idx;
    }

    void Bicho::setCaminhada(long v)
    {
        this->caminhada = v;
    }

    void Bicho::imprimeDados()
    {
        std::cout << "\n--------------------------------";
        std::cout << "\nCamada de Entrada (Camada0):";
        for( size_t i = 0 ; i < this->camadaEntrada.size();i++)
        {
            std::cout << "\nValor    : " << camadaEntrada.at(i)->getValor();
            std::cout << "\nBias     : " << camadaEntrada.at(i)->getBias();
            for(int j = 0 ; j < camadaEntrada.at(i)->getQEntradas() ;j++)
            {
                std::cout << "\nPeso(i,j)(" << i << "," << j << " : " << camadaEntrada.at(i)->getPesoEntrada(j);
            }
            std::cout << "\n";
        }

        std::cout << "\nCamada Oculta 1 (Camada1):";
        for( size_t i = 0 ; i < this->camadaO1.size();i++)
        {
            std::cout << "\nValor    : " << camadaO1.at(i)->getValor();
            std::cout << "\nBias     : " << camadaO1.at(i)->getBias();
            for(int j = 0 ; j < camadaO1.at(i)->getQEntradas() ;j++)
            {
                std::cout << "\nPeso(i,j)(" << i << "," << j << " : " << camadaO1.at(i)->getPesoEntrada(j);
            }
            std::cout << "\n";
        }

        std::cout << "\nCamada Oculta 2 (Camada2):";
        for( size_t i = 0 ; i < this->camadaO2.size();i++)
        {
            std::cout << "\nValor    : " << camadaO2.at(i)->getValor();
            std::cout << "\nBias     : " << camadaO2.at(i)->getBias();
            for(int j = 0 ; j < camadaO2.at(i)->getQEntradas() ;j++)
            {
                std::cout << "\nPeso(i,j)(" << i << "," << j << " : " << camadaO2.at(i)->getPesoEntrada(j);
            }
            std::cout << "\n";
        }

        std::cout << "\nCamada de Saida (Camada3):";
        for( size_t i = 0 ; i < this->camadaSaida.size();i++)
        {
            std::cout << "\nValor    : " << camadaSaida.at(i)->getValor();
            std::cout << "\nBias     : " << camadaSaida.at(i)->getBias();
            for(int j = 0 ; j < camadaSaida.at(i)->getQEntradas() ;j++)
            {
                std::cout << "\nPeso(i,j)(" << i << "," << j << " : " << camadaSaida.at(i)->getPesoEntrada(j);
            }
            std::cout << "\n";
        }
    }

    void Bicho::somaPontuacao(long valor)
    {
        this->pontuacao+=valor;
    }
    long Bicho::getPontuacao()
    {
        return this->pontuacao;
    }
    void Bicho::setPontuacao(long p)
    {
        this->pontuacao=p;
    }

    void Bicho::reiniciar(sf::Vector2f posicao)
    {
        setPosition(posicao);
        setDirecao('B');
        this->pontuacao=0;
        this->energia=this->energiaInicial;
        this->caminhada=0;
        this->corpo.setFillColor(sf::Color::Yellow);
    }

    rn::Neuronio* Bicho::getNeuronio(int idCamada, int idNeuronio)
    {
        if(idCamada==0)
        {
            return this->camadaEntrada.at(idNeuronio);
        }
        else if(idCamada==1)
        {
            return this->camadaO1.at(idNeuronio);
        }
        else if(idCamada==2)
        {
            return this->camadaO2.at(idNeuronio);
        }
        else if(idCamada==3)
        {
            return this->camadaSaida.at(idNeuronio);
        }

        std::cout << "impossivel recuperar valor de bicho::getValorCamadaRN\n";
        return nullptr;
    }

    Bicho* Bicho::crossOver(Bicho* gerador2)
    {
        Bicho* filho = new Bicho;
        const int corte = 3.0;
        rn::Neuronio *origem;

        for(int i = 0 ; i < filho->camadaEntrada.size() ;i++)
        {
            double aleatorio = geradorAleatorio(0.0,10.0);
            if(aleatorio<5.0) origem = this->camadaEntrada.at(i);
            else origem = gerador2->camadaEntrada.at(i);

            rn::Neuronio *destino = filho->camadaEntrada.at(i);
            rn::copiaConfigNeuronio(origem,destino);
        }

        for(int i = 0 ; i < filho->camadaO1.size() ;i++)
        {
            double aleatorio = geradorAleatorio(0.0,10.0);
            if(aleatorio<5.0) origem = this->camadaO1.at(i);
            else origem = gerador2->camadaO1.at(i);

            rn::Neuronio *destino = filho->camadaO1.at(i);
            rn::copiaConfigNeuronio(origem,destino);
        }

        for(int i = 0 ; i < filho->camadaO2.size() ;i++)
        {
            double aleatorio = geradorAleatorio(0.0,10.0);
            if(aleatorio<5.0) origem = this->camadaO2.at(i);
            else origem = gerador2->camadaO2.at(i);

            rn::Neuronio *destino = filho->camadaO2.at(i);
            rn::copiaConfigNeuronio(origem,destino);
        }

        for(int i = 0 ; i < filho->camadaSaida.size() ;i++)
        {
            double aleatorio = geradorAleatorio(0.0,10.0);
            if(aleatorio<5.0) origem = this->camadaSaida.at(i);
            else origem = gerador2->camadaSaida.at(i);
            rn::Neuronio *destino = filho->camadaSaida.at(i);
            rn::copiaConfigNeuronio(origem,destino);
        }

        return filho;
    }



    void Bicho::setId(int id)
    {
        this->id = id;
        this->txtIdent->setString(std::to_string(this->id));
    }

    int Bicho::getId()
    {
        return this->id;
    }



    void Bicho::setPosicaoAnterior(sf::Vector2f pos)
    {
        this->posAnterior = pos;
    }
    sf::Vector2f Bicho::getPosicaoAnterior()
    {
        return this->posAnterior;
    }

    std::vector<rn::Neuronio*>& Bicho::getCamada(int idCamada)
    {
        if(idCamada==0)
        {
            return this->camadaEntrada;
        }
        else if(idCamada==1)
        {
            return this->camadaO1;
        }
        else if(idCamada==2)
        {
            return this->camadaO2;
        }
        else if(idCamada==3)
        {
            return this->camadaSaida;
        }

    }

void Bicho::salvarDadosRedeNeural(std::string nomeArquivo) {

    std::vector<std::vector<rn::Neuronio*>> camadas{this->camadaEntrada,this->camadaO1,this->camadaO2,this->camadaSaida};
    int tam = camadas.size();
    std::ofstream arquivo(nomeArquivo);

    arquivo << this->getId() << "\n";
    for(int c = 0 ; c < tam ;c++)
    {
        int t = camadas.at(c).size();
        for(int i = 0 ; i < t ;i++)
        {
            arquivo << camadas.at(c).at(i)->getBias() << ";";
            int n = camadas.at(c).at(i)->getQEntradas();
            for(int j = 0 ; j < n ;j++)
            {
                arquivo <<camadas.at(c).at(i)->getPesoEntrada(j) << ";";
            }
            arquivo << "\n";
        }

    }
    arquivo.close();
}



void Bicho::carregarDadosRedeNeural(std::string nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.good()) return;

    std::string linha;
    std::string valor;
    std::vector<double> valores;

    // Ler os bias e pesos da rede
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        while (std::getline(ss, valor, ';'))
        {
              valores.push_back(std::stod(valor));
        }
    }
    arquivo.close();

    std::vector<std::vector<rn::Neuronio*>> camadas{this->camadaEntrada,this->camadaO1,this->camadaO2,this->camadaSaida};

    int id=0;
    int tam = camadas.size();
    this->setId((int)valores.at(id++));
    for(int c = 0 ; c < tam ;c++)
    {
        int t = camadas.at(c).size();
        for(int i = 0 ; i < t ;i++)
        {
            camadas.at(c).at(i)->setBias(valores.at(id++));
            int n = camadas.at(c).at(i)->getQEntradas();
            for(int j = 0 ; j < n ; j++)
            {
                camadas.at(c).at(i)->setPesoEntrada(j,valores.at(id++));
            }
        }
    }
}





