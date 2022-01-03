// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, DNI/pasaporte: 48261534-J (IG GIM+GIADE, curso 21-22)

#ifndef LATA_PEONES_HPP
#define LATA_PEONES_HPP


#include "matrices-tr.h"
#include "grafo-escena.h"
#include "materiales-luces.h"


class Lata: public NodoGrafoEscena
{
    private:
        int brand;  // <0 window-icon, ==0 pepsi, >0 cocacola
    public:

        Lata(int type);

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class Peones: public NodoGrafoEscena
{
    private:
    
    public:

        Peones();

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class LataPeones: public NodoGrafoEscena
{
    private:
    
    public:

        LataPeones();

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class VariasLatasPeones: public NodoGrafoEscena
{
    private:
    
    public:

        VariasLatasPeones();

        unsigned leerNumParametros() const ;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};



#endif