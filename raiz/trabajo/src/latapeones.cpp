// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, DNI/pasaporte: 48261534-J (IG GIM+GIADE, curso 21-22)

#include "ig-aux.h"
#include "matrices-tr.h"
#include "latapeones.h"
#include "malla-ind.h"
#include "malla-revol.h"
//Incluímos para calcular la revolución
#include <cmath>
#define PI 3.14159265
#define TIEMPO_ACCION 4



Lata::Lata(int type){
    brand = type;

    float ambiente = 0.3;

    agregar(EntradaNGE(MAT_Escalado(4,4,4)));

    ponerCentroOC({0,0.6450,0});
    
    agregar(EntradaNGE( new Material(ambiente,2,2,10.0)));
    
    MallaRevolPLY *tapa_sup = new MallaRevolPLY("../recursos/plys/lata-psup.ply",13);
    tapa_sup->ponerColor(Tupla3f(0.5,0.5,0.5));
    agregar(EntradaNGE(  tapa_sup ));
    
    MallaRevolPLY *tapa_inf = new MallaRevolPLY("../recursos/plys/lata-pinf.ply",13);
    tapa_inf->ponerColor(Tupla3f(0.5,0.5,0.5));
    agregar(EntradaNGE(  tapa_inf ));


    float k_dif =1.5, k_pse =1.5, exp_pse = 10.0;
    if(brand<0){
        agregar(EntradaNGE( new Material(new Textura("../recursos/imgs/window-icon.jpg"),ambiente,k_dif, k_pse, exp_pse) ));
    }else if(brand == 0){
        agregar(EntradaNGE( new Material(new Textura("../recursos/imgs/lata-pepsi.jpg"),ambiente,k_dif, k_pse, exp_pse) ));
    }else{
        agregar(EntradaNGE( new Material(new Textura("../recursos/imgs/lata-coke.jpg"), ambiente,k_dif, k_pse, exp_pse) ));
    }
    
    agregar(EntradaNGE(  new MallaRevolPLY("../recursos/plys/lata-pcue.ply",13) ));

}



unsigned Lata::leerNumParametros() const{
    return 0;
}

void Lata::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
}




Peones::Peones(){
    agregar(EntradaNGE(MAT_Traslacion(2.2,1.4,0)));
    
    agregar(EntradaNGE( new Material(0.3,0.1,1,10.0) ));

    MallaRevolPLY *peon_negro = new MallaRevolPLY("../recursos/plys/peon.ply",33);
    peon_negro->ponerColor(Tupla3f(0.0,0.0,0.0));
    peon_negro->ponerNombre("Peón negro");
    peon_negro->ponerIdentificador(14);    // el último número es la práctica (P4) y el primero el orden cronológico en el que lo hice 
    agregar(EntradaNGE(  peon_negro ));
    
    agregar(EntradaNGE(MAT_Traslacion(-2.2,0,0)));

    agregar(EntradaNGE( new Material(0.3,1,0.0,10.0) ));
    MallaRevolPLY *peon_blanco = new MallaRevolPLY("../recursos/plys/peon.ply",33);
    peon_blanco->ponerColor(Tupla3f(1,1,1));
    peon_blanco->ponerNombre("Peón blanco");
    peon_blanco->ponerIdentificador(24);    // el último número es la práctica (P4) y el primero el orden cronológico en el que lo hice 
    agregar(EntradaNGE(  peon_blanco ));

    agregar(EntradaNGE(MAT_Traslacion(-2.2,0,0)));

    NodoGrafoEscena * nodo_peon_madera = new NodoGrafoEscena();
    nodo_peon_madera->ponerNombre("Peón madera");
    nodo_peon_madera->ponerIdentificador(34);    // el último número es la práctica (P4) y el primero el orden cronológico en el que lo hice 
    nodo_peon_madera->agregar(EntradaNGE( new Material(new TexturaXY("../recursos/imgs/text-madera.jpg"),0.3,1,3,30.0) ));
    nodo_peon_madera->agregar(EntradaNGE(  new MallaRevolPLY("../recursos/plys/peon.ply",33) ));

    agregar(nodo_peon_madera);

}



unsigned Peones::leerNumParametros() const{
    return 0;
}

void Peones::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
}



LataPeones::LataPeones(){
    Lata * cocacola = new Lata(+1);
    cocacola->ponerNombre("Lata de Coca-Cola");
    cocacola->ponerIdentificador(44);    // el último número es la práctica (P4) y el primero el orden cronológico en el que lo hice 

    agregar(EntradaNGE( cocacola));

    agregar(EntradaNGE(MAT_Traslacion(2.2,0,2)));
    agregar(EntradaNGE(new Peones()));
}



unsigned LataPeones::leerNumParametros() const{
    return 0;
}

void LataPeones::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
}





VariasLatasPeones::VariasLatasPeones(){
    agregar(EntradaNGE(MAT_Traslacion(6,0,0)));

    Lata * ugr = new Lata(-1);
    ugr->ponerNombre("Lata de la UGR");
    ugr->ponerIdentificador(15);        // el último número es la práctica (P5) y el primero el orden cronológico en el que lo hice (este fue al primero en ponerle identificador)
    agregar(EntradaNGE( ugr));

    agregar(EntradaNGE(MAT_Traslacion(-3,0,0)));

    Lata * pepsi = new Lata(0);
    pepsi->ponerNombre("Lata de Pepsi");
    pepsi->ponerIdentificador(25);    // el último número es la práctica (P5) y el primero el orden cronológico en el que lo hice 
    agregar(EntradaNGE( pepsi));

    agregar(EntradaNGE(MAT_Traslacion(-3,0,0)));
    agregar(EntradaNGE(new LataPeones()));
}



unsigned VariasLatasPeones::leerNumParametros() const{
    return 0;
}

void VariasLatasPeones::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
}


