

#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP

#include "matrices-tr.h"
#include "grafo-escena.h"
#include "materiales-luces.h"


/*
    Archivo .h que representará las clases de los objetos jerárquicos
*/



class CompuertaEngranada : public NodoGrafoEscena
{
    private:
        Matriz4f * rotacion_eje;
        bool frontal;       // Las frontales se cierran primero
    public:

        CompuertaEngranada(unsigned compuerta);

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class BrazoDron : public NodoGrafoEscena
{
    private:
        Matriz4f * rotacion_helices;
    public:

        BrazoDron(int angulo);

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};


class Dron : public NodoGrafoEscena
{
    private:
        std::vector<BrazoDron*> brazo;
        std::vector<CompuertaEngranada*> compuerta;
    public:

        Dron();

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );


};

class Carga : public NodoGrafoEscena
{
    private:
        Matriz4f * contraccion_cuerda;
        Matriz4f * elevacion_carga;
    public:

        Carga();

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );


};


class DronCarga : public NodoGrafoEscena
{
    private:
        Dron * dron;
        Carga * carga;
        Matriz4f * circuito;

    public:

        DronCarga();

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );


};


#endif 
