// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, DNI/pasaporte: 48261534-J (IG GIM+GIADE, curso 21-22)

// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"

//Incluímos para calcular la revolución
#include <cmath>
#define PI 3.14159265


using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   unsigned m = perfil.size();

   //Práctica 4
   vector<Tupla3f> nor_aristas;
   vector<float> dist;
   float suma_dist=0;
   
   for(unsigned i=0; i<m-1; i++){
      // Normales
      Tupla3f v_aux = perfil[i+1]-perfil[i];
      Tupla3f v(-v_aux(1),v_aux(0),0);    // Rotamos 90º
      nor_aristas.push_back(v);

      // Textura
      dist.push_back(sqrt((perfil[i+1]-perfil[i]).lengthSq()));
      suma_dist+=dist[i];
   }

   // Normales
   vector<Tupla3f> perf_norver;
   perf_norver.push_back(nor_aristas[0]);
   for(unsigned i=1; i<nor_aristas.size(); i++){
      Tupla3f n = nor_aristas[i-1]+nor_aristas[i];
      n.normalized();
      perf_norver.push_back(n);
   }
   perf_norver.push_back(nor_aristas[nor_aristas.size()-1]);

   // Texturas
   vector<float> t;
   float suma_parcial=0;
   for(unsigned i=0; i<m; i++){
      t.push_back(suma_parcial/suma_dist);
      if(i<m-1)
         suma_parcial+=dist[i];
   }



   // COMPLETAR: Práctica 2: completar: creación de la malla....
   // Vértices
   for(unsigned i=0; i<num_copias; i++){
      for(int j=0; j<m; j++){
         // P4 Normales
         float x=perf_norver[j](0)*cos((2*PI*i)/(num_copias-1)),
               y=perf_norver[j](1),
               z=perf_norver[j](0)*sin((2*PI*i)/(num_copias-1));   
         
         Tupla3f q(x,y,z);
         nor_ver.push_back(q);
      
         // P4 Texturas
         cc_tt_ver.push_back({1.0- ((float)i)/((float) num_copias-1), 1-t[j]});         
         
         // P2 Vértices
         x=perfil[j](0)*cos((2*PI*i)/(num_copias-1));
         y=perfil[j](1);
         z=perfil[j](0)*sin((2*PI*i)/(num_copias-1));   // Usamos la componente X de perfil porque nos da "el radio"
         
         Tupla3f p(x,y,z);
         vertices.push_back(p);
      }
   }

   // Triángulos
   for(unsigned i=0; i<num_copias-1; i++){
      for(unsigned j=0; j<m-1; j++){
         int k = i*m+j;

         Tupla3i triangulo1(k, k+m, k+m+1);
         triangulos.push_back(triangulo1);
         
         Tupla3i triangulo2(k,k+m+1,k+1);
         triangulos.push_back(triangulo2);

      }
   }




}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   std::vector<Tupla3f> perfil;
   LeerVerticesPLY(nombre_arch, perfil);

   inicializar(perfil, nperfiles);
}


// -----------------------------------------------------------------------------

Cilindro::Cilindro( 
         const int num_verts_per,     // número de vértices del perfil original (m)
         const unsigned nperfiles     // número de perfiles (n)
         ): MallaRevol()
{
   ponerNombre( std::string("Cilindro"));

   std::vector<Tupla3f> perfil;
   
   // Base inferior: ponemos un punto en el perfil en el (0,0,0)
   Tupla3f base_inf(0,0,0);
   perfil.push_back(base_inf);

   // Puntos laterales del perfil que no son tapas
   for(unsigned i=0; i<num_verts_per-2; i++){     // Restamos dos por el vértice de la tapa y de la base
      Tupla3f p(1,((float) i)/((float) (num_verts_per-3)),0);
      perfil.push_back(p);
   }

   // Base superior: ponemos un punto en el perfil en el (0,1,0)
   Tupla3f base_sup(0,1,0);
   perfil.push_back(base_sup);

   inicializar(perfil, nperfiles);
}


// -----------------------------------------------------------------------------

Cono::Cono( 
         const int num_verts_per,     // número de vértices del perfil original (m)
         const unsigned nperfiles     // número de perfiles (n)
         ): MallaRevol()
{
   ponerNombre( std::string("Cono"));
   std::vector<Tupla3f> perfil;
   
   Tupla3f base_inf(0,0,0);
   perfil.push_back(base_inf);

   for(int i=0; i<num_verts_per-1; i++){
      Tupla3f p(1-((float) i)/((float) num_verts_per-2),((float) i)/((float) num_verts_per-2),0);
      perfil.push_back(p);
   }

   inicializar(perfil, nperfiles);
}


// -----------------------------------------------------------------------------

Esfera::Esfera( 
         const int num_verts_per,     // número de vértices del perfil original (m)
         const unsigned nperfiles     // número de perfiles (n)
         ): MallaRevol()
{
   ponerNombre( std::string("Esfera"));
   std::vector<Tupla3f> perfil;
   
   for(int i=0; i<num_verts_per; i++){
      Tupla3f p(sin(i*PI/(num_verts_per-1)),cos(PI-i*PI/(num_verts_per-1)),0);
      perfil.push_back(p);
   }

   inicializar(perfil, nperfiles);
}


// -----------------------------------------------------------------------------

TuboPrisma::TuboPrisma( int nperfiles): MallaRevol()
{
   ponerNombre( std::string("Tubo"));
   std::vector<Tupla3f> perfil = {{0,0,0}, {0.02,0,0}, {0.02,0.5,0}, {0,0.5,0}};


   inicializar(perfil, nperfiles);
}