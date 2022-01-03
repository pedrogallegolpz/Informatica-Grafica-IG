// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, DNI/pasaporte: 48261534-J (IG GIM+GIADE, curso 21-22)


#include "ig-aux.h"
#include "matrices-tr.h"
#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"
//Incluímos para calcular la revolución
#include <cmath>
#define PI 3.14159265
#define TIEMPO_ACCION 4


CompuertaEngranada::CompuertaEngranada(unsigned compuerta){
    switch(compuerta){
        case 0:
            agregar(EntradaNGE(MAT_Traslacion(0.4,0,0.15)));
            ponerNombre("Compuerta Engranada 0");
            ponerIdentificador(13);
            frontal = false;
            break;
        case 1:
            agregar(EntradaNGE(MAT_Traslacion(0.2+0.4,0,0.35)));
            agregar(EntradaNGE(MAT_Rotacion(180,0,1,0)));
            ponerNombre("Compuerta Engranada 1");
            ponerIdentificador(23);
            frontal = false;
            break;
        case 2:
            agregar(EntradaNGE(MAT_Traslacion(0.4,0,0.2+0.15)));
            agregar(EntradaNGE(MAT_Rotacion(90,0,1,0)));
            ponerNombre("Compuerta Engranada 2");
            ponerIdentificador(33);
            frontal = true;
            break;
        case 3:
            agregar(EntradaNGE(MAT_Traslacion(0.6,0,0.15)));
            agregar(EntradaNGE(MAT_Rotacion(270,0,1,0)));
            ponerNombre("Compuerta Engranada 3");
            ponerIdentificador(43);
            frontal = true;
            break;
    }
    

    rotacion_eje = leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(0,0.1,0,0))));
    
    agregar(EntradaNGE(MAT_Traslacion(0,-0.1,0)));
    agregar(EntradaNGE(MAT_Escalado(0.2,0.2,0.2)));
    agregar(EntradaNGE( new Compuerta()));
}



unsigned CompuertaEngranada::leerNumParametros() const{
    return 1;
}

void CompuertaEngranada::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    int segundos_accion = TIEMPO_ACCION;
    float ratio = 1/( (float) segundos_accion);
    unsigned tiempo = t_sec; // Hacemos cast a entero
    float t_relativo = t_sec - segundos_accion*((int) tiempo/(3*segundos_accion));
    unsigned tiempo_relativo;

    switch(iParam){
        case 0:
            if(tiempo%(segundos_accion*3)>=segundos_accion){

                t_relativo = t_relativo - ((float) segundos_accion);
                tiempo_relativo = t_relativo;
                
                if( (tiempo_relativo%(segundos_accion*2) < (segundos_accion/2) || tiempo_relativo%(segundos_accion*2) >= 3*(segundos_accion/2)) && frontal){
                    v1 = (-90+0)/2+(0-(-90))*sin(2*PI*ratio*t_relativo + PI/2)/2;
                    *rotacion_eje = MAT_Rotacion(v1,{0.1,0,0});
                }else if((tiempo_relativo%(segundos_accion*2) >= (segundos_accion/2) && tiempo_relativo%(segundos_accion*2) < 3*(segundos_accion/2)) && !frontal){
                    v1 = (-90+0)/2+(0-(-90))*sin(2*PI*ratio*t_relativo - PI/2)/2;
                    *rotacion_eje = MAT_Rotacion(v1,{0.1,0,0});
                }
            }
            break;
    }    
}



BrazoDron::BrazoDron(int angulo){
    switch(angulo){
        case 135:
            agregar(EntradaNGE(MAT_Traslacion(0.1,0.2,0.1)));
            ponerNombre("Brazo Dron 0");
            ponerIdentificador(53);
            break;
        case 225:
            agregar(EntradaNGE(MAT_Traslacion(0.1,0.2,0.4)));
            ponerNombre("Brazo Dron 1");
            ponerIdentificador(63);
            break;
        case 315:
            agregar(EntradaNGE(MAT_Traslacion(0.9,0.2,0.4)));
            ponerNombre("Brazo Dron 2");
            ponerIdentificador(73);
            break;
        case 45:
            agregar(EntradaNGE(MAT_Traslacion(0.9,0.2,0.1)));
            ponerNombre("Brazo Dron 3");
            ponerIdentificador(83);
            break;
    }
    agregar(EntradaNGE(MAT_Rotacion(angulo,0,1,0)));            
    agregar(EntradaNGE(MAT_Rotacion(270,0,0,1)));
    agregar(EntradaNGE(MAT_Rotacion(45,0,1,0)));
    TuboPrisma *p = new TuboPrisma(5);
    p->ponerColor({1,1,1});
    agregar(EntradaNGE(p));

    agregar(EntradaNGE(MAT_Rotacion(225,0,1,0)));        // Para arreglar la siguiente rotación

    agregar(EntradaNGE(MAT_Traslacion(0,0.5,0.02)));
    agregar(EntradaNGE(MAT_Escalado(0.2,0.2,0.4)));

    rotacion_helices = leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(0,0,0,1))));
    Helices * h = new Helices();
    h->ponerColor({0.4,0.4,0.4});
    agregar(EntradaNGE(h));
}



unsigned BrazoDron::leerNumParametros() const{
    return 1;
}

void BrazoDron::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    switch(iParam){
        case 0:
            v1 = 360*2*t_sec;
            *rotacion_helices = MAT_Rotacion(v1,0,0,1);
            break;
    }    
}


Dron::Dron(){
    
    // Centramos
    agregar(EntradaNGE(MAT_Traslacion(-0.5,-0.2,-0.25)));

    
    // Creamos las compuertas
    compuerta.push_back(new CompuertaEngranada(0));
    compuerta.push_back(new CompuertaEngranada(1));
    compuerta.push_back(new CompuertaEngranada(2));
    compuerta.push_back(new CompuertaEngranada(3));
    for(unsigned i=0; i<compuerta.size(); i++){
        compuerta[i]->ponerColor({0.2,0.2,0.2});
        agregar(EntradaNGE(compuerta[i]));
    }
    

    // Creamos los brazos
    brazo.push_back(new BrazoDron(45));
    brazo.push_back(new BrazoDron(315));
    brazo.push_back(new BrazoDron(225));
    brazo.push_back(new BrazoDron(135));
    for(unsigned i=0; i<brazo.size(); i++){
        agregar(EntradaNGE(brazo[i]));
    }

    // Creamos el cuerpo del Dron
    CuerpoDron *d = new CuerpoDron();
    d->ponerColor({0.7,0.7,0.7});
    d->ponerNombre("Cuerpo Dron");
    d->ponerIdentificador(93);
    agregar(EntradaNGE(d));

}

unsigned Dron::leerNumParametros() const{
    return 2;
}

void Dron::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    switch(iParam){
        case 0:
            for(unsigned i=0;i<brazo.size();i++){
                brazo[i]->actualizarEstadoParametro(0,t_sec);
            }
            break;
        case 1:
            
            for(unsigned i=0;i<compuerta.size();i++){
                compuerta[i]->actualizarEstadoParametro(0,t_sec);
            }
            
            break;
    }
    
    
}




Carga::Carga(){
    
    elevacion_carga = leerPtrMatriz(agregar(EntradaNGE(MAT_Traslacion(0,0,0))));

    // Creamos el cubo (carga)
    agregar(EntradaNGE(MAT_Escalado(0.09,0.09,0.09)));   // Hacemos el cuadrado suficientemente pequeño para que quepa por la compuerta. Con esto la cuerda tiene longitud 2 (como máximo)
    Cubo *c = new Cubo();
    c->ponerColor({0.7,0.1,0.1});
    agregar(EntradaNGE(c));
    

    agregar(EntradaNGE(MAT_Traslacion(0,1,0)));  // Ponemos la cuerda encima del cubo

    contraccion_cuerda = leerPtrMatriz(agregar(EntradaNGE(MAT_Escalado(1,1,1))));   // Esta matriz reescalará la cuerda
    agregar(EntradaNGE(MAT_Escalado((1.0/0.09),4.0*(1.0/0.09),(1.0/0.09))));   // Alargamos la cuerda

    // Creamos la cuerda
    TuboPrisma *p = new TuboPrisma(5);
    p->ponerColor({0.6,0.3,0});
    p->ponerNombre("Cuerda");
    p->ponerIdentificador(103);
    agregar(EntradaNGE(p));

}

unsigned Carga::leerNumParametros() const{
    return 1;
}

void Carga::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    int segundos_accion = TIEMPO_ACCION;
    float ratio = 1/( (float) segundos_accion);

    int tiempo = t_sec; // Cast a int

    float v1;
    switch(iParam){
        case 0:
            if(tiempo%(segundos_accion*3)<segundos_accion){
                v1 = 1.9/2+(-1.9)*sin(2*PI*ratio*t_sec+PI/2)/2;
                *elevacion_carga = MAT_Traslacion(0,1.9-v1,0);
                *contraccion_cuerda = MAT_Escalado(1,(0.1+v1)/2.0,1);
            }
            break;
    }
    
    
}


DronCarga::DronCarga(){
    // Circuito
    circuito = leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(0,0,1,0))));

    // Lo subimos de altura de forma que la carga se quede en el suelo (y=0)
    agregar(EntradaNGE(MAT_Traslacion(0,0.1,1)));

    // Creamos la carga
    carga = new Carga();
    carga->ponerNombre("Carga");
    carga->ponerIdentificador(123);
    agregar(EntradaNGE(carga));

    agregar(EntradaNGE(MAT_Traslacion(0,1.95,0)));

    // Creamos el Dron
    dron = new Dron();
    dron->ponerNombre("Dron");
    dron->ponerIdentificador(113);
    agregar(EntradaNGE(dron));
}

unsigned DronCarga::leerNumParametros() const{
    return 3;
}

void DronCarga::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    float vel_angular;
    switch(iParam){
        case 0:
            vel_angular = 1.0/(360.0/10);
            v1 = 360*vel_angular*t_sec;
            *circuito = MAT_Rotacion(v1,0,1,0);
            break;
        case 1:
            dron->actualizarEstadoParametro(0,t_sec);
            break;
        case 2:
            dron->actualizarEstadoParametro(1,t_sec); 
            carga->actualizarEstadoParametro(0,t_sec);                                               
            break;
    }
    
    
}

////////////////////////////////////////////
//
//  EJER 1 EXTRAS
//
////////////////////////////////////////////


ConoPunta::ConoPunta(){
    

    // Lo subimos de altura de forma que la carga se quede en el suelo (y=0)

    agregar(EntradaNGE(MAT_Traslacion(0,1.3,0)));

    // Creamos la carga
    Cono * cono = new Cono(4,10);
    agregar(EntradaNGE(MAT_Escalado(0.14,0.25,0.14)));
    agregar(EntradaNGE(cono));

}




GrafoEstrellaX::GrafoEstrellaX(unsigned n){
    assert(n>1);
    
    // Circuito
    rotacion = leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(0,0,1,0))));

    // Ponemos "recta" la figura
    agregar(EntradaNGE(MAT_Rotacion(360.0/((float) n),{1,0,0})));

    // Creamos la carga
    ConoPunta * cono = new ConoPunta();
    for(unsigned i=0; i<n; i++){
        agregar(EntradaNGE(MAT_Rotacion(360.0/((float) n),{1,0,0})));

        // Creamos la carga
        agregar(EntradaNGE(cono));
    }

    agregar(EntradaNGE(MAT_Escalado(1,2.6,2.6)));
    agregar(EntradaNGE(MAT_Rotacion(270,{0,1,0})));
    agregar(EntradaNGE(MAT_Rotacion(90,{0,0,1})));
    agregar(EntradaNGE(MAT_Traslacion(-0.5,-0.5,0)));

    // Creamos el Dron
    EstrellaZ * star = new EstrellaZ(n);
    agregar(EntradaNGE(star));
}

unsigned GrafoEstrellaX::leerNumParametros() const{
    return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    float vel_angular;
    switch(iParam){
        case 0:
            vel_angular = 2.5;
            v1 = 360*vel_angular*t_sec;
            *rotacion = MAT_Rotacion(v1,1,0,0);
            break;
    }
    
    
}



////////////////////////////////////////////
//
//  EJER 2 EXTRAS
//
////////////////////////////////////////////

CaraCubo::CaraCubo(){
   
    //Subimos
    agregar(EntradaNGE(MAT_Traslacion(0,1,0)));

    agregar(EntradaNGE(MAT_Escalado(2,2,2)));
    agregar(EntradaNGE(MAT_Traslacion(-0.5,0,-0.5)));

    // Creamos el Dron
    RejillaY * rejilla = new RejillaY(5,5);
    agregar(EntradaNGE(rejilla));

    // Posicionamos encima de la rejilla
    agregar(EntradaNGE(MAT_Traslacion(0.5,0,0.5)));
    agregar(EntradaNGE(MAT_Escalado(0.2,0.2,0.2)));


    Cubo * cubo = new Cubo();

    rotacion = leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(0,{1,0,0}))));
    agregar(EntradaNGE(MAT_Escalado(1,1.5,1)));

    agregar(EntradaNGE(MAT_Traslacion(0,0.5,0)));
    agregar(EntradaNGE(MAT_Escalado(0.5,0.5,0.5)));

    agregar(EntradaNGE(cubo));

    
}

unsigned CaraCubo::leerNumParametros() const{
    return 1;
}

void CaraCubo::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    float vel_angular;
    switch(iParam){
        case 0:
            vel_angular = 0.5;
            v1 = 360*vel_angular*t_sec;
            *rotacion = MAT_Rotacion(v1,0,1,0);
            break;
    }
    
    
}


GrafoCubos::GrafoCubos(){
   
    // Creamos la cara
    cara = new CaraCubo();

    for(int i=0; i<4; i++){
        agregar(EntradaNGE(MAT_Rotacion(180,{0,0,1})));
        agregar(EntradaNGE(cara));
    }

    agregar(EntradaNGE(MAT_Rotacion(90,{1,0,0})));


    for(int i=0; i<4; i++){
        agregar(EntradaNGE(MAT_Rotacion(90,{0,0,1})));
        agregar(EntradaNGE(cara));
    }

}

unsigned GrafoCubos::leerNumParametros() const{
    return 1;
}

void GrafoCubos::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros()); 
    switch(iParam){
        case 0:
            cara->actualizarEstadoParametro(0,t_sec);
            break;
    }
    
    
}



