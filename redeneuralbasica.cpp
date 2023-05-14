#include "redeneuralbasica.h"

#include <iostream>
#include <vector>
#include <cmath>

namespace rn{

    double Neuronio::funcao_sigmoid(double v)
    {
        return 1/(1+std::exp(-(v)));
    }

    double Neuronio::funcao_Binarystep(double v, double limiar)
    {
        return v<limiar?0:1;
    }

    double Neuronio::funcao_relu(double v)
    {
        return v<0?0:v;
    }

    double Neuronio::funcao_leakyRelu(double v, double alfa)
    {
        double aux = alfa*v;
        return v>aux?v:aux;
    }

    double Neuronio::funcao_tanh(double v)
    {
        return ( std::exp(v) - std::exp(-v) ) / ( std::exp(v) + std::exp(-v) ); //f(x) = (e^x - e^-x) / (e^x + e^-x)
    }



    Neuronio::Neuronio()
    {
        setValor(0.0);
    }

    Neuronio::Neuronio(int nEntradas, std::string funcaoAtivacao)
    {
        this->qEntrada=nEntradas;
        neuroEntrada = new Neuronio*[nEntradas];
        pesoEntrada = new double[nEntradas];
        setValor(0.0);
        setFuncaoAtivacao(funcaoAtivacao);
    }



    Neuronio::~Neuronio()
    {
        delete[] neuroEntrada;
        delete[] pesoEntrada;
    }



    Neuronio::Neuronio(const Neuronio& other)
    {
        valor = other.valor;
        bias = other.bias;
        qEntrada = other.qEntrada;
        neuroEntrada = new Neuronio*[qEntrada];
        pesoEntrada = new double[qEntrada];
        codFuncaoAtivacao = other.codFuncaoAtivacao;

        for (int i = 0; i < qEntrada; i++) {
            neuroEntrada[i] = other.neuroEntrada[i]; // verificar aqui um novo jeito de copiar os neuronios de entrada
            pesoEntrada[i] = other.pesoEntrada[i];
        }
    }


    // sobrecarga do operador de =
    Neuronio& Neuronio::operator=(const Neuronio& other)
    {
      if (this != &other) {
         valor = other.valor;
        bias = other.bias;
        qEntrada = other.qEntrada;
        codFuncaoAtivacao = other.codFuncaoAtivacao;

        delete[] pesoEntrada;
        pesoEntrada = new double[qEntrada];
        std::copy(other.pesoEntrada, other.pesoEntrada + qEntrada, pesoEntrada);

        for (int i = 0; i < qEntrada; i++) {
          neuroEntrada[i] = new Neuronio(*other.neuroEntrada[i]);
        }
      }
      return *this;
    }


    void Neuronio::setFuncaoAtivacao(std::string nomeFuncao)
    {
        this->codFuncaoAtivacao = converterNomeFuncao(nomeFuncao);
    }

    int Neuronio::converterNomeFuncao(std::string funcao)
    {
        if(funcao=="none") return 0;
        else if(funcao=="sigmoid") return 1;
        else if (funcao=="step") return 2;
        else if (funcao=="relu") return 3;
        else if (funcao=="leaky") return 4;
        else if (funcao=="tanh") return 5;
        else return -1;
    }
    std::string Neuronio::nomeFuncao(int codFuncao)
    {
        if (codFuncao==0) return "none";
        else if(codFuncao==1) return "sigmoid";
        else if (codFuncao==2) return "step";
        else if (codFuncao==3) return "relu";
        else if (codFuncao==4) return "leaky";
        else if(codFuncao==5) return "tanh";
        else return "erro";
    }

    void Neuronio::setValor(double valor)
    {
        this->valor = valor;
    }

    double Neuronio::getValor() const
    {
        return this->valor;
    }

    int Neuronio::getQEntradas() const
    {
        return this->qEntrada;
    }

    void Neuronio::calcularSaida()
    {
        int tam = this->qEntrada;
        double soma = 0.0;

        for(int i = 0 ; i < tam ;i++)
        {
            Neuronio *nTemp = this->neuroEntrada[i];
            double vEntrada = nTemp->getValor();
            double peso = getPesoEntrada(i);
            soma+=vEntrada*peso;
        }
        soma+=getBias();
        this->valor = funcaoAtivacao(soma,nomeFuncao(this->codFuncaoAtivacao));
    }



    int Neuronio::getCodFuncaoAtivacao(){return this->codFuncaoAtivacao;}
    void Neuronio::setCodFuncaoAtivacao(int codigo){this->codFuncaoAtivacao=codigo;}



    double Neuronio::funcaoAtivacao(double v,std::string funcao)
    {
        if(funcao=="none")
        {
            return v;
        }
        else if(funcao=="sigmoid")
        {
            return funcao_sigmoid(v);
        }
        else if(funcao=="step")
        {
            return funcao_Binarystep(v,0);
        }
        else if(funcao=="relu")
        {
            return funcao_relu(v);
        }
        else if(funcao=="leaky")
        {
            return funcao_leakyRelu(v,0.01);
        }
        else if(funcao=="tanh")
        {
            return funcao_tanh(v);
        }
        else
        {
            return v;
        }
    }

    void Neuronio::setPesoEntrada(int id, double valor)
    {
        this->pesoEntrada[id] = valor;
    }

    double Neuronio::getPesoEntrada(int id) const
    {
        return this->pesoEntrada[id];
    }

    void Neuronio::setBias(double valor)
    {
        this->bias = valor;
    }

    double Neuronio::getBias() const
    {
        return this->bias;
    }

    void Neuronio::setNeuroEntrada(int id, Neuronio* n,double peso)
    {
        this->neuroEntrada[id] = n;
        setPesoEntrada(id,peso);
    }

    Neuronio* Neuronio::getNeuronioEntrada(int id)
    {
        return this->neuroEntrada[id];
    }

    void Neuronio::imprimePesos() const
    {
        std::cout << "\nPesos de entrada do Neuronio\n";
        if(this->qEntrada==0) std::cout << "Nenhum\n\n";
        for(int i = 0 ; i < this->qEntrada ;i++)
        {
            std::cout << "(" << i << ")=" << getPesoEntrada(i) << '\n';
        }
        std::cout << "\n";
    }

    void Neuronio::imprimeValores() const
    {
        std::cout << "Valores do Neuronio\n";
        std::cout << "Bias  = " << getBias();
        std::cout << "\nValor = " << getValor();
        std::cout << "\n";
    }


void SoftMax(std::vector<Neuronio*> &camada)
{
    int tam = camada.size();
    double somaExp = 0.0;
    for (int i = 0 ; i < tam ; i++)
    {
        Neuronio* n = camada.at(i);
        somaExp+= std::exp(n->getValor());
    }
    for (int i = 0 ; i < tam ; i++)
    {
        Neuronio* n = camada.at(i);
        n->setValor(std::exp(n->getValor())/somaExp);
    }

}

std::vector<rn::Neuronio*>& criarCamada(int quantNeuronios, std::string funcao, std::vector<rn::Neuronio*> &camadaAnterior)
{

    static std::vector<Neuronio*> camada;
    camada.clear();
    int nCamadaAnterior = camadaAnterior.size();
    for(int i = 0 ; i < quantNeuronios;i++)
    {
        Neuronio *o = new Neuronio(nCamadaAnterior,funcao);
        for(int j = 0 ; j < nCamadaAnterior;j++)
        {
            o->setNeuroEntrada(j,camadaAnterior.at(j),0.0);
        }
        camada.push_back(o);
    }

return camada;
}


std::vector<Neuronio*>& criarCamadaEntrada(int quantNeuronios, std::string funcao)
{

    static std::vector<Neuronio*> camada;
    camada.clear();
    for(int i = 0 ; i < quantNeuronios;i++)
    {
        Neuronio *o = new Neuronio(0,funcao);
        camada.push_back(o);
    }
    return camada;
}

void imprimeCamada(std::vector<Neuronio*> &camada)
{
    int n = camada.size();
    for(int i = 0 ; i < n ;i++)
    {
        camada.at(i)->imprimeValores();
        camada.at(i)->imprimePesos();
    }
}


void calcularSaidaCamada(std::vector<Neuronio*> &camada)
{
    int n = camada.size();
    for(int i = 0 ; i < n ;i++)
    {
        camada.at(i)->calcularSaida();
    }
}

void setBiasCamada(std::vector<Neuronio*> &camada, std::vector<double> &vBias)
{
    int n = camada.size();
    for(int i = 0 ; i < n;i++)
    {
        camada.at(i)->setBias(vBias.at(i));
    }
}

void setPesoCamada(std::vector<Neuronio*> &camada, std::vector<std::vector<double>> vPesos)
{
    int n = camada.size();
    for(int i = 0 ; i < n ; i++)
    {
        int t = camada.at(i)->getQEntradas();
        for(int j = 0 ; j < t;j++)
        {
            camada.at(i)->setPesoEntrada(j,vPesos.at(i).at(j));
        }
    }
}

void setValorCamada(std::vector<Neuronio*> &camada, std::vector<double> vValores)
{
    int n = camada.size();
    for(int i = 0 ; i < camada.size();i++)
    {
        camada.at(i)->setValor(vValores.at(i));
    }
}


int getIndiceMaximoCamada(std::vector<Neuronio*> &camada){
    int t = camada.size();
    int indice = -1;
    double maximo = 0.0;
    for(int i = 0 ; i < t ; i++)
    {
        double valor = camada.at(i)->getValor();
        if(i==0)
        {
            maximo = valor;
            indice = 0;
        }
        else if(valor>maximo)
        {
            maximo = valor;
            indice = i;
        }
    }
    return indice;
}

void copiaConfigNeuronio(Neuronio *origem, Neuronio *destino)
{
    destino->setBias(origem->getBias());
    destino->setValor(origem->getValor());
    destino->setQEntradas(origem->getQEntradas());
    destino->setCodFuncaoAtivacao(origem->getCodFuncaoAtivacao());

    for(int i = 0 ; i < origem->getQEntradas(); i++)
    {
        destino->setPesoEntrada(i,origem->getPesoEntrada(i));
    }
}

void Neuronio::setQEntradas(int q)
    {
        this->qEntrada = q;
    }

}//namespace

