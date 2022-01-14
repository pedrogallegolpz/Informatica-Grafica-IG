// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, DNI/pasaporte: 48261534-J (IG GIM+GIADE, curso 21-22)

// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "matrices-tr.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"

#define PI 3.14159265

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETADO: práctica 3: recorrer las entradas y visualizar cada nodo.
   // ........
   // Guardamos el antiguo color del cauce y fijamos el color del objeto actual en el cauce

   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada des de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)

   const Tupla4f color_previo = leerFijarColVertsCauce(cv);
   cv.cauce_act->pushMM();
   
   Material * material_previo = cv.iluminacion ? cv.material_act : nullptr;

   for(unsigned i=0; i<entradas.size(); i++){
      switch(entradas[i].tipo){
         case TipoEntNGE::objeto:
            entradas[i].objeto->visualizarGL(cv);
            break;

         case TipoEntNGE::transformacion:
            cv.cauce_act->compMM( *entradas[i].matriz);
            break;

         case TipoEntNGE::material:
            if(cv.iluminacion){
               cv.material_act=entradas[i].material;
               cv.material_act->activar(cv);
            }            
            break;
         case TipoEntNGE::noInicializado:
            break;
      }

   }
   cv.cauce_act->popMM();

   if(material_previo != nullptr){
      cv.material_act = material_previo;
      cv.material_act->activar(cv);
   }
   // restaurar el color previamente fijado
   glColor4fv( color_previo );
}




// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// ignora el colo o identificador del nodo, ignora las entradas de tipo material
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)




// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETADO: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   unsigned indice = entradas.size();
   entradas.push_back(entrada);

   return indice ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETADO: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........(sustituir 'return nullptr' por lo que corresponda)
   assert(indice < entradas.size());
   assert(entradas[indice].tipo == TipoEntNGE::transformacion);

   return entradas[indice].matriz ;


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........
   if(!centro_calculado){
      Tupla3f centro(0,0,0);
      unsigned num_objetos = 0;

      Matriz4f modelado = MAT_Ident();

      for(unsigned i=0; i<entradas.size(); i++){
         // Para cada entrada
      
         switch(entradas[i].tipo){
            case TipoEntNGE::objeto:
               // Si es un objeto
               entradas[i].objeto->calcularCentroOC();
               centro = centro + modelado*entradas[i].objeto->leerCentroOC();  // Sumamos su centro
               num_objetos++;
               break;

            case TipoEntNGE::transformacion:
               // Si es una transformación
               modelado = modelado*(*entradas[i].matriz);
               break;
         }
      }

      centro = centro / ((float)num_objetos);    // Calculamos el punto medio

      ponerCentroOC(centro);
   

      centro_calculado = true;
   }
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........
   calcularCentroOC();

   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........
   if(ident_busc == leerIdentificador()){
      (*objeto) = this;
      centro_wc = mmodelado*leerCentroOC();

      return true;
   }

   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
   Matriz4f modelado=mmodelado;
   for(unsigned i=0; i<entradas.size(); i++){
      // Para cada entrada
   
      switch(entradas[i].tipo){
         case TipoEntNGE::objeto:
            // Si es un objeto
            if(entradas[i].objeto->buscarObjeto(ident_busc, modelado, objeto, centro_wc))
               return true;
            break;

         case TipoEntNGE::transformacion:
            // Si es una transformación
            modelado = modelado*(*entradas[i].matriz);
            break;
      }
   }

   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}




// Práctica 4

NodoCubo::NodoCubo(){
   agregar(EntradaNGE(new Material(new Textura("../recursos/imgs/window-icon.jpg"),0.3,1,1,10)));
   agregar(EntradaNGE(new Cubo24()));
}

unsigned NodoCubo::leerNumParametros() const{
    return 0;
}

void NodoCubo::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());   
}



///////////////////////////////////
/// EXAMEN


CuboAux::CuboAux(float tmin, float tmax, float T){
    Cubo * cubo = new Cubo();
    agregar(EntradaNGE(MAT_Traslacion(1,0,1)));
    agregar(EntradaNGE(cubo));
}

unsigned CuboAux::leerNumParametros() const{
    return 0;
}

void CuboAux::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
}

VariosCubos::VariosCubos(int n,float tmin, float tmax, float T){
   t_min = tmin;
   t_max = tmax;
   periodo = T;

   CuboAux * cubo = new CuboAux(tmin, tmax, T);
   agregar(EntradaNGE(MAT_Traslacion(-1,0,-1)));
   agregar(EntradaNGE(cubo));

   for(int i=0; i<n-1; i++){
      agregar(EntradaNGE(MAT_Traslacion(2,0,2)));

      if(i%2==0){
         rotacion.push_back(leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(90-(tmin+tmax)/2.0,{0,1,0})))));
      }else{
         rotacion.push_back(leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion(360-(90-(tmin+tmax)/2.0),{0,1,0})))));
      }
      agregar(EntradaNGE(cubo));
   }

    
}

unsigned VariosCubos::leerNumParametros() const{
    return 1;
}

void VariosCubos::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    float vel_angular;
    switch(iParam){
        case 0:
            vel_angular = 1/periodo;
            v1 = (t_min+t_max)/2.0 + 0.5*(t_max-t_min)*sin(2*PI*vel_angular*t_sec);
            for(unsigned i=0; i<rotacion.size(); i++){
               if(i%2==0){
                  *(rotacion[i]) = MAT_Rotacion(90-v1,{0,1,0});
               }else{
                  *(rotacion[i]) = MAT_Rotacion(360-(90-v1),{0,1,0});
               }
            }
            break;
    }
}





///////////////////////////////////
/// EJERCICIO ADICIONAL P4
NodoDiscoP4::NodoDiscoP4(){
   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero");
   agregar(MAT_Traslacion(0,1.25,0));

   agregar(EntradaNGE( new Material(new Textura("../recursos/imgs/cuadricula.jpg"),0.3,1,3,30.0) ));
   agregar( new MallaDiscoP4(true) );
   
   agregar(MAT_Traslacion(0,-2.5,0));

   agregar( new MallaDiscoP4(false) );

}

unsigned NodoDiscoP4::leerNumParametros() const{
    return 0;
}

void NodoDiscoP4::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
}



///////////////////////////////////
/// EJERCICIO ADICIONAL P5
GrafoEsferasP5::GrafoEsferasP5(){
   const unsigned 
      n_filas_esferas= 8,
      n_esferas_x_fila= 5 ;
   
   const float
      e = 0.4/n_esferas_x_fila ;

   agregar( MAT_Escalado( e,e,e ));

   for( unsigned i = 0 ; i < n_filas_esferas ; i++ ){
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ ){
         Esfera * esfera = new Esfera(30,30) ;

         //añadido
         esfera->ponerIdentificador(5500+i*n_esferas_x_fila+j);
         esfera->ponerNombre("esfera número "+std::to_string(i+1)+" de la fila número "+ std::to_string(j+1));
         
         
         fila_esferas->agregar( MAT_Traslacion( 2.2, 0.0, 0.0 ));
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar( MAT_Traslacion( 0.0, 0.0, 5.0 ));
   }
}


unsigned GrafoEsferasP5::leerNumParametros() const{
    return 0;
}

void GrafoEsferasP5::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
}


GrafoEsferasP5_2::GrafoEsferasP5_2(){
   const unsigned
      n_filas_esferas= 8,
      n_esferas_x_fila= 5 ;
   const float e = 2.5/n_esferas_x_fila ;
   
   agregar( MAT_Escalado( e, e, e ));
   
   for( unsigned i = 0 ; i < n_filas_esferas ; i++ ){
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      fila_esferas->agregar( MAT_Traslacion( 3.0, 0.0, 0.0 ));
      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ ){
         Esfera * esfera = new Esfera(30,30) ;

         //añadido
         esfera->ponerIdentificador(6500+i*n_esferas_x_fila+j);
         esfera->ponerNombre("esfera número "+std::to_string(i+1)+" de la fila número "+ std::to_string(j+1));
         
         
         fila_esferas->agregar( MAT_Traslacion( 2.5, 0.0, 0.0 ));
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar( MAT_Rotacion( 360.0/n_filas_esferas, { 0.0, 1.0, 0.0 }));
   }
}

unsigned GrafoEsferasP5_2::leerNumParametros() const{
   return 0;
}

void GrafoEsferasP5_2::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
}





////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////
////            EXAMEN P45
////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
P4Malla::P4Malla(){
   ponerNombre("Malla examen práctica 4 (21-22)");
    
   vertices =
      {   { 0.0, 1.0, 0.0 }, { +1.0, 0.0, +1.0  }, { +1.0, 0.0, -1.0 },  // mira hacia X+ e Y+
         { 0.0, 1.0, 0.0 }, { -1.0, 0.0, -1.0  }, { -1.0, 0.0, +1.0 },  // mira hacia X- e Y+
         { 0.0, 1.0, 0.0 }, { -1.0, 0.0, +1.0  }, { +1.0, 0.0, +1.0 },  // mira hacia Z+ e Y+
         { 0.0, 1.0, 0.0 }, { +1.0, 0.0, -1.0  }, { -1.0, 0.0, -1.0 }   // mira hacia Z- e Y+
      } ;

   triangulos =
      {  {0,1,2}, {3,4,5}, {6,7,8}, {9,10,11}
      } ;


   nor_tri =
      {
         {1.0/sqrt(2),1.0/sqrt(2),0.0},{-1.0/sqrt(2),1.0/sqrt(2),0.0},{0.0,1.0/sqrt(2),1.0/sqrt(2)},{0.0,1.0/sqrt(2),-1.0/sqrt(2)}
      };

   nor_ver =
      {
         {1.0/sqrt(2),1.0/sqrt(2),0.0},{1.0/sqrt(2),1.0/sqrt(2),0.0},{1.0/sqrt(2),1.0/sqrt(2),0.0},
         {-1.0/sqrt(2),1.0/sqrt(2),0.0},{-1.0/sqrt(2),1.0/sqrt(2),0.0},{-1.0/sqrt(2),1.0/sqrt(2),0.0},
         {0.0,1.0/sqrt(2),1.0/sqrt(2)},{0.0,1.0/sqrt(2),1.0/sqrt(2)},{0.0,1.0/sqrt(2),1.0/sqrt(2)},
         {0.0,1.0/sqrt(2),-1.0/sqrt(2)},{0.0,1.0/sqrt(2),-1.0/sqrt(2)},{0.0,1.0/sqrt(2),-1.0/sqrt(2)}
      };


   // Textura
   for(unsigned i=0; i<vertices.size(); i++){
      if(i%3==0){
         cc_tt_ver.push_back({0.5,1.0});
      }else if(i%3==1){
         cc_tt_ver.push_back({0.0,0.0});
      }else{
         cc_tt_ver.push_back({1.0,0.0});
      }
   }

}





P4Nodo::P4Nodo(){
   agregar(EntradaNGE(new Material(new Textura("../recursos/imgs/textura-examen.jpg"),0.8,0.3,0.3,15)));
   agregar(EntradaNGE(new P4Malla()));
}

unsigned P4Nodo::leerNumParametros() const{
   return 0;
}

void P4Nodo::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
}



////////////////////////////////////////////////////////////////////////
P5Malla::P5Malla(){
   ponerNombre("Malla examen práctica 5 (21-22)");
    
   vertices =
      {  { 1.0, 1.0, 1.0 },  //Y+
         { 1.0, 1.0, -1.0 }, 
         { 1.0, -1.0, 1.0  }, // Y-
         { 1.0, -1.0, -1.0 }   
      } ;

   triangulos =
      {  {0,2,1},{1,2,3}
      } ;

   nor_tri ={{1.0,0.0,0.0},{1.0,0.0,0.0}};
   nor_ver={{1.0,0.0,0.0},{1.0,0.0,0.0},{1.0,0.0,0.0},{1.0,0.0,0.0}};

   ponerCentroOC({1.0,0.0,0.0});
}




P5Nodo::P5Nodo(){
   /*
      0 -> Este X+
      1 -> Norte Z-
      2 -> Oeste X-
      3 -> Sur Z+
   */
   P5Malla * malla = new P5Malla();
   malla->ponerIdentificador(4500);  
   agregar(EntradaNGE(malla));

   agregar(EntradaNGE(MAT_Rotacion(90,{0,1,0})));

   malla = new P5Malla();
   malla->ponerIdentificador(4501);  
   agregar(EntradaNGE(malla));


   agregar(EntradaNGE(MAT_Rotacion(90,{0,1,0})));

   malla = new P5Malla();
   malla->ponerIdentificador(4502);  
   agregar(EntradaNGE(malla));


   agregar(EntradaNGE(MAT_Rotacion(90,{0,1,0})));

   malla = new P5Malla();
   malla->ponerIdentificador(4503);  
   agregar(EntradaNGE(malla));
   
}

unsigned P5Nodo::leerNumParametros() const{
   return 0;
}

void P5Nodo::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
}

