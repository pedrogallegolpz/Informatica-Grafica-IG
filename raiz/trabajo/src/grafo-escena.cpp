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
   const Tupla4f color_previo = leerFijarColVertsCauce(cv);
   cv.cauce_act->pushMM();

   for(unsigned i=0; i<entradas.size(); i++){
      switch(entradas[i].tipo){
         case TipoEntNGE::objeto:
            entradas[i].objeto->visualizarGL(cv);
            break;
         case TipoEntNGE::transformacion:
            cv.cauce_act->compMM( *entradas[i].matriz);
            break;
   

   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada des de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)


         case TipoEntNGE::material:
            break;
         case TipoEntNGE::noInicializado:
            break;
      }

   }
   cv.cauce_act->popMM();


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


   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........


   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........


   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}


///////////////////////////////////
/// EXAMEN


CuboAux::CuboAux(float tmin, float tmax, float T){
   


    Cubo * cubo = new Cubo();

    //rotacion = leerPtrMatriz(agregar(EntradaNGE(MAT_Rotacion((tmin+tmax)/2.0,{0,1,0}))));

    agregar(EntradaNGE(MAT_Traslacion(1,0,1)));

    agregar(EntradaNGE(cubo));

    
}

unsigned CuboAux::leerNumParametros() const{
    return 0;
}

void CuboAux::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam<leerNumParametros());
    float v1;
    float vel_angular;
    switch(iParam){
        case 0:
           
            break;
    }
    
    
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
