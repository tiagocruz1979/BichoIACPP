#ifndef REDENEURALBASICA_H_INCLUDED
#define REDENEURALBASICA_H_INCLUDED

#include <iostream>
#include <vector>
#include <cmath>

namespace rn{


class Neuronio
{
    double valor= 0.0;
    double bias = 0.0;
    int qEntrada=0;
    Neuronio **neuroEntrada;
    double *pesoEntrada;
    int codFuncaoAtivacao = 0;

    double funcao_sigmoid(double v);
    double funcao_Binarystep(double v, double limiar=0);
    double funcao_relu(double v);
    double funcao_leakyRelu(double v, double alfa);
    double funcao_tanh(double v);


public:
    Neuronio();
    Neuronio(int nEntradas, std::string funcaoAtivacao);
    Neuronio(Neuronio const &other);
    Neuronio& operator=(const Neuronio& other);
    ~Neuronio();
    void setFuncaoAtivacao(std::string nomeFuncao);
    int converterNomeFuncao(std::string funcao);
    std::string nomeFuncao(int codFuncao);
    int getCodFuncaoAtivacao();
    void setCodFuncaoAtivacao(int codigo);
    void setValor(double valor);
    double getValor() const;
    void calcularSaida();
    double funcaoAtivacao(double v,std::string funcao);
    void setPesoEntrada(int id, double valor);
    double getPesoEntrada(int id) const;
    void setBias(double valor);
    double getBias() const;
    int getQEntradas() const;
    void setQEntradas(int q);
    void setNeuroEntrada(int id, Neuronio* n,double peso);
    Neuronio* getNeuronioEntrada(int id);
    void imprimePesos() const;
    void imprimeValores() const;
};


void SoftMax(std::vector<Neuronio*> &camada);
std::vector<Neuronio*>& criarCamada(int quantNeuronios, std::string funcao, std::vector<Neuronio*> &camadaAnterior);
std::vector<Neuronio*>& criarCamadaEntrada(int quantNeuronios, std::string funcao);
void imprimeCamada(std::vector<Neuronio*> &camada);
void calcularSaidaCamada(std::vector<Neuronio*> &camada);
void setBiasCamada(std::vector<Neuronio*> &camada, std::vector<double> &vBias);
void setPesoCamada(std::vector<Neuronio*> &camada, std::vector<std::vector<double>> pesos);
void setValorCamada(std::vector<Neuronio*> &camada, std::vector<double> vValores);
int getIndiceMaximoCamada(std::vector<Neuronio*> &camada);
void copiaConfigNeuronio(Neuronio *origem, Neuronio *destino);


} //namespace rn
#endif // REDENEURALBASICA_H_INCLUDED
