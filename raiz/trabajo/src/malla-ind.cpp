// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

//#include <set>   // std::set
#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......


}




// ----------------------------------------------------------------------------


void MallaInd::visualizarGL( ContextoVis & cv )
{


   using namespace std ;
   assert( cv.cauce_act != nullptr );

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETADO: práctica 1: si el puntero 'array_verts' es nulo, crear el objeto ArrayVerts
   //   * en el constructor se dan los datos de la tabla de coordenadas de vértices (tabla 'vertices')
   //   * después hay que invocar a 'fijarIndices', usando el formato y datos de la tabla de triángulos ('triangulos')
   //   * si las tablas 'col_ver', 'cc_tt_ver' o 'nor_ver' no están vacías, hay que invocar los métodos 
   //     'fijarColores', 'fijarCoordText' y 'fijarNormales', como corresponda.
   if(array_verts==nullptr){
      array_verts = new ArrayVertices(GL_FLOAT, 3, vertices.size(), vertices.data());
   }
   
   // El formate de los índices es Tuple3i, luego habrá 3*num_tuplas índices  
   array_verts->fijarIndices(GL_UNSIGNED_INT, 3*triangulos.size(), triangulos.data());

   if(!col_ver.empty())      
      array_verts->fijarColores(GL_FLOAT, 3, col_ver.data());

   if(!cc_tt_ver.empty())
      array_verts->fijarCoordText(GL_FLOAT, 3, cc_tt_ver.data());

   if(!nor_ver.empty())
      array_verts->fijarNormales(GL_FLOAT, nor_ver.data());


   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'
   //   ** inmediato con una llamada : usar método 'visualizarGL_MI_DAE' de 'ArrayVerts'
   //   ** diferido (con un VAO)     : usar método 'visualizarGL_MD_VAO' de 'ArrayVerts'
   // (en cualquier caso hay que pasar como parámetro el tipo de primitiva adecuada a una malla de triángulos).
   // .....
   switch(cv.modo_envio){
      case ModosEnvio::inmediato_begin_end:         
         array_verts->visualizarGL_MI_BVE(GL_TRIANGLES);
         break;
      case ModosEnvio::inmediato_drawelements:        
         array_verts->visualizarGL_MI_DAE(GL_TRIANGLES);
         break;
      case ModosEnvio::diferido_vao:        
         array_verts->visualizarGL_MD_VAO(GL_TRIANGLES);
         break;
   }

   // restaurar el color previamente fijado
   glColor4fv( color_previo );
}






// *****************************************************************************


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   // Comprobación de que el archivo debe de ser un nombre relativo que nombrará
   // alguno de los archivos de la carpeta ply en recursos
   //    Ejemplo:    ../recursos/ply/beethoven.ply // completar
   LeerPLY(nombre_arch, vertices, triangulos);     // Carpeta recursos


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................



}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}

// ****************************************************************************
// Clase 'CuboTejado'

CuboTejado::CuboTejado()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +0.0, +2.0, +0.0 }, // 8
      } ;

   for( Tupla3f & v : vertices )
      v = 2.0f*v +Tupla3f({0.0,2.0,0.0});



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)   // quito cara superior
         {2,3,8}, {3,7,8}, {7,6,8}, {6,2,8}, // añado tejado 

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}
// ****************************************************************************
// Clase 'Tetraedro'

Tetraedro::Tetraedro()
:  MallaInd( "Tetraedro 4 vértices" )
{

   vertices =
      {  { +1.0, 0.0, 0.0 }, // 0
         { -0.5, +0.866, 0.0 }, // 1
         { -0.5, -0.866, 0.0 }, // 2
         { 0.0, 0.0, +1.0 }, // 3
      } ;



   triangulos =
      {  {0,1,2},          // base
         {0,1,3},          // Primera cara (según sentido antihorario)
         {1,2,3},          // Segunda cara (según sentido antihorario)
         {2,0,3},          // Tercera cara (según sentido antihorario)
      } ;

}
// ****************************************************************************
// Clase 'Cubo Colores

CuboColores::CuboColores()
:  MallaInd( "cubo colores 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   // Colores
   for(int i = 0; i<vertices.size(); i++){
      float a,b,c;
      a=(vertices[i][0]+1)/2;
      b=(vertices[i][1]+1)/2;
      c=(vertices[i][2]+1)/2;

      col_ver.push_back({a,b,c});
   }
}

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'Helices'

Helices::Helices()
:  MallaInd( "Helices" )
{

   // Sobran vértices puesto que las esquinas interiores de cada hélice coinciden
   // con dos vértices de una de su lado y del otro. Es decir, los vértices del eje
   // de las hélices se repiten dos veces.
   vertices =
      {  {  0.05,  -0.05, 0.05 }, // Hélice 1.front 1
         { 1.0, -0.2, 0.05},      // Hélice 1.front 2
         {  1.0,  0.2, 0.05 },    // Hélice 1.front 3
         {  0.05,  0.05, 0.05 },  // Hélice 1.front 4
         {  0.05,  -0.05, -0.05 }, // Hélice 1.back 1
         { 1.0, -0.2, -0.05},      // Hélice 1.back 2
         {  1.0,  0.2, -0.05 },    // Hélice 1.back 3
         {  0.05,  0.05, -0.05 },  // Hélice 1.back 4

         {  0.05, 0.05, 0.05},    // Hélice 2.front 1
         {  0.2,  1, 0.05 },      // Hélice 2.front 2
         {  -0.2,  1, 0.05 },     // Hélice 2.front 3
         {  -0.05,  0.05, 0.05 }, // Hélice 2.front 4
         {  0.05, 0.05, -0.05},    // Hélice 2.back 1
         {  0.2,  1, -0.05 },      // Hélice 2.back 2
         {  -0.2,  1, -0.05 },     // Hélice 2.back 3
         {  -0.05,  0.05, -0.05 }, // Hélice 2.back 4

         {  -0.05, 0.05, 0.05},    // Hélice 3.front 1
         {  -1,  0.2, 0.05 },      // Hélice 3.front 2
         {  -1,  -0.2, 0.05 },     // Hélice 3.front 3
         {  -0.05,  -0.05, 0.05 }, // Hélice 3.front 4
         {  -0.05, 0.05, -0.05},    // Hélice 3.back 1
         {  -1,  0.2, -0.05 },      // Hélice 3.back 2
         {  -1,  -0.2, -0.05 },     // Hélice 3.back 3
         {  -0.05,  -0.05, -0.05 }, // Hélice 3.back 4

         {  -0.05, -0.05, 0.05},    // Hélice 4.front 1
         {  -0.2,  -1, 0.05 },      // Hélice 4.front 2
         {  0.2,  -1, 0.05 },       // Hélice 4.front 3
         {  0.05,  -0.05, 0.05 },   // Hélice 4.front 4
         {  -0.05, -0.05, -0.05},    // Hélice 4.back 1
         {  -0.2,  -1, -0.05 },      // Hélice 4.back 2
         {  0.2,  -1, -0.05 },       // Hélice 4.back 3
         {  0.05,  -0.05, -0.05 },   // Hélice 4.back 4
      } ;


   // Triángulos
   // Como consecuencia de repetirse los vértices de las hélices
   // también se repiten algunos triángulos
   for(int i=0; i<4; i++){
      // H+
      triangulos.push_back({0+8*i,1+8*i,2+8*i});
      triangulos.push_back({2+8*i,3+8*i,0+8*i});
      // H-
      triangulos.push_back({4+8*i,5+8*i,6+8*i});
      triangulos.push_back({6+8*i,7+8*i,4+8*i});
      
      // H base
      triangulos.push_back({0+8*i,4+8*i,5+8*i});
      triangulos.push_back({5+8*i,1+8*i,0+8*i});

      // H lateral
      triangulos.push_back({1+8*i,5+8*i,6+8*i});
      triangulos.push_back({6+8*i,2+8*i,1+8*i});

      // H techo
      triangulos.push_back({2+8*i,6+8*i,7+8*i});
      triangulos.push_back({7+8*i,3+8*i,2+8*i});
   }

   // Centro triangulos
   for(int i=0; i<2; i++){
      triangulos.push_back({ 0+4*i, 3+4*i, 8+4*i});
      triangulos.push_back({ 8+4*i,11+4*i, 0+4*i});
      triangulos.push_back({ 0+4*i,11+4*i,16+4*i});
      triangulos.push_back({16+4*i,19+4*i, 0+4*i});
      triangulos.push_back({ 0+4*i,19+4*i,24+4*i});
      triangulos.push_back({24+4*i,27+4*i, 0+4*i});
   }

   
}

// -----------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'Cuerpo Dron'

CuerpoDron::CuerpoDron()
:  MallaInd( "Cuerpo Dron" )
{

   float altura = 0.4;
   vertices =
      {  {0.4,0,0.15},{0.2,0,0.1},{0.2,0.1,0},{0.2,altura-0.1,0},{0.2,altura,0.1},{0.5,altura,0.25},
         {0.4,0,0.15},{0.1,0,0.2},{0,0.1,0.2},{0,altura-0.1,0.2},{0.1,altura,0.2},{0.5,altura,0.25},

         {0.4,0,0.35},{0.1,0,0.3},{0,0.1,0.3},{0,altura-0.1,0.3},{0.1,altura,0.3},{0.5,altura,0.25},
         {0.4,0,0.35},{0.2,0,0.4},{0.2,0.1,0.5},{0.2,altura-0.1,0.5},{0.2,altura,0.4},{0.5,altura,0.25},

         {0.6,0,0.35},{0.8,0,0.4},{0.8,0.1,0.5},{0.8,altura-0.1,0.5},{0.8,altura,0.4},{0.5,altura,0.25},
         {0.6,0,0.35},{0.9,0,0.3},{1,0.1,0.3},{1,altura-0.1,0.3},{0.9,altura,0.3},{0.5,altura,0.25},

         {0.6,0,0.15},{0.9,0,0.2},{1,0.1,0.2},{1,altura-0.1,0.2},{0.9,altura,0.2},{0.5,altura,0.25},
         {0.6,0,0.15},{0.8,0,0.1},{0.8,0.1,0},{0.8,altura-0.1,0},{0.8,altura,0.1},{0.5,altura,0.25},

         {0.4,0,0.15},{0.2,0,0.1},{0.2,0.1,0},{0.2,altura-0.1,0},{0.2,altura,0.1},{0.5,altura,0.25}
   };


   // Triángulos
   int num_copias = 9;
   int m=6;
   for(int i=0; i<num_copias-1; i++){
      for(int j=0; j<m-1; j++){
         int k = i*m+j;

         Tupla3i triangulo1(k, k+m, k+m+1);
         triangulos.push_back(triangulo1);
         
         Tupla3i triangulo2(k,k+m+1,k+1);
         triangulos.push_back(triangulo2);

      }
   }  

   
}



// -----------------------------------------------------------------------------------------------

// ****************************************************************************
// Clase 'Compuerta'

Compuerta::Compuerta()
:  MallaInd( "Compuerta" )
{
   vertices = {   {0,0,0},{1,0,0},{1,0.5,0},{0,0.5,0},
                  {0,0,0.05},{1,0,0.05},{1,0.5,0.05},{0,0.5,0.05}
               };


   // Triángulos
   triangulos.push_back({0,3,1});
   triangulos.push_back({3,2,1});

   
   vertices =
      {  { 0, 0, -0.05 }, // 0
         { 0, 0, 0 }, // 1
         { 0, 0.5, -0.05 }, // 2
         { 0, 0.5, 0 }, // 3
         { +1.0, 0, -0.05 }, // 4
         { +1.0, 0, 0 }, // 5
         { +1.0, 0.5, -0.05 }, // 6
         { +1.0, 0.5, 0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}



// -----------------------------------------------------------------------------------------------
