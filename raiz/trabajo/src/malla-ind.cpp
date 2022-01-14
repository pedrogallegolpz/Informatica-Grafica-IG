// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, DNI/pasaporte: 48261534-J (IG GIM+GIADE, curso 21-22)

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
#define PI 3.14159265


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
// visualizar normales
void MallaInd::visualizarNormales()
{
   using namespace std ;

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  
   if ( array_verts_normales == nullptr )
   {  
      for( unsigned i = 0 ; i < vertices.size() ; i++ )
      {  
         segmentos_normales.push_back( vertices[i] );
         segmentos_normales.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
      }
      array_verts_normales = new ArrayVertices( GL_FLOAT, 3, segmentos_normales.size(), segmentos_normales.data() );
   }

   array_verts_normales->visualizarGL_MI_DAE( GL_LINES );
   CError();
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

   // COMPLETADO: Práctica 4: creación de la tabla de normales de triángulos
   // ....
   for(unsigned i=0; i<triangulos.size(); i++){
      Tupla3f p = vertices[triangulos[i](0)];
      Tupla3f q = vertices[triangulos[i](1)];
      Tupla3f r = vertices[triangulos[i](2)];

      Tupla3f a = q-p;
      Tupla3f b = r-p;

      Tupla3f m = a.cross(b);

      if(m(0)!=0 or m(1)!=0 or m(2)!=0){
         m = m.normalized();
      }

      nor_tri.push_back(m);
   }
}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETADO: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   calcularNormalesTriangulos();

   nor_ver.resize(vertices.size(),Tupla3f(0,0,0));

   for(unsigned i=0; i<triangulos.size(); i++){
      nor_ver[triangulos[i](0)] = nor_ver[triangulos[i](0)]+nor_tri[i];
      nor_ver[triangulos[i](1)] = nor_ver[triangulos[i](1)]+nor_tri[i];
      nor_ver[triangulos[i](2)] = nor_ver[triangulos[i](2)]+nor_tri[i];
   }

   for(unsigned i=0; i<nor_ver.size(); i++){
      if(nor_ver[i](0)!=0 or nor_ver[i](1)!=0 or nor_ver[i](2)!=0){
         nor_ver[i] = nor_ver[i].normalized();
      }
   }

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

   if ( cv.visualizando_normales )
   {  visualizarNormales(  );
      return ;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETADO: práctica 1: si el puntero ' ' es nulo, crear el objeto ArrayVerts
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
      array_verts->fijarCoordText(GL_FLOAT, 2, cc_tt_ver.data());
   
   if(!nor_ver.empty())
      array_verts->fijarNormales(GL_FLOAT, nor_ver.data());


   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'visualizarGL
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
   calcularNormales();


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
      {  {0,1,2}, {1,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,4,1}, {1,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,2,4}, {4,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   /*
   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;
   */

   calcularNormales();
}

// Clase 'Cubo24

Cubo24::Cubo24()
:  MallaInd( "cubo 24 vértices" )
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

	 { -1.0, -1.0, -1.0 }, // 0 +8
         { -1.0, -1.0, +1.0 }, // 1 +8
         { -1.0, +1.0, -1.0 }, // 2 +8
         { -1.0, +1.0, +1.0 }, // 3 +8
         { +1.0, -1.0, -1.0 }, // 4 +8
         { +1.0, -1.0, +1.0 }, // 5 +8
         { +1.0, +1.0, -1.0 }, // 6 +8
         { +1.0, +1.0, +1.0 }, // 7 +8

	 { -1.0, -1.0, -1.0 }, // 0 +16
         { -1.0, -1.0, +1.0 }, // 1 +16
         { -1.0, +1.0, -1.0 }, // 2 +16 
         { -1.0, +1.0, +1.0 }, // 3 +16
         { +1.0, -1.0, -1.0 }, // 4 +16
         { +1.0, -1.0, +1.0 }, // 5 +16
         { +1.0, +1.0, -1.0 }, // 6 +16
         { +1.0, +1.0, +1.0 } // 7 +16
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)
         {8,13,9}, {8,12,13}, // Y-
         {10,11,15}, {10,15,14}, // Y+ (+2)
         {16,22,20}, {16,18,22}, // Z-
         {17,21,23}, {17,23,19}  // Z+ (+1)
      } ;

   cc_tt_ver=
     {{0,1}, // 0
      {1,1}, // 1
      {0,0}, // 2
      {1,0}, // 3
      {1,1}, // 4
      {0,1}, // 5
      {1,0}, // 6
      {0,0}, // 7

      {0,0}, // 0 +8
      {1,0}, // 1 +8
      {1,0}, // 2 +8
      {0,0}, // 3 +8
      {0,1}, // 4 +8
      {1,1}, // 5 +8
      {1,1}, // 6 +8
      {0,1}, // 7 +8

      {1,1}, // 0 +16
      {0,1}, // 1 +16
      {1,0}, // 2 +16 
      {0,0}, // 3 +16
      {0,1}, // 4 +16
      {1,1}, // 5 +16
      {0,0}, // 6 +16
      {1,0} // 7 +16
     };


   calcularNormales();
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

   calcularNormales();

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
   for(unsigned i = 0; i<vertices.size(); i++){
      float a,b,c;
      a=(vertices[i][0]+1.0)/2.0;
      b=(vertices[i][1]+1.0)/2.0;
      c=(vertices[i][2]+1.0)/2.0;

      col_ver.push_back({a,b,c});
   }

   calcularNormales();

}

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'EstrellaZ'

EstrellaZ::EstrellaZ(unsigned n)
:  MallaInd( "EstrellaZ" )
{
   assert(n>1);
   
   vertices.push_back({0.5,0.5,0});
   for(unsigned i=0; i<2*n; i++){
      if(i%2==0){
         vertices.push_back({0.5+cos(2*PI*i/(2*n))/2, 0.5+sin(2*PI*i/(2*n))/2, 0});
      }else{
         vertices.push_back({0.5+cos(2*PI*i/(2*n))/6, 0.5+sin(2*PI*i/(2*n))/6, 0});
      }
   }

   // Triángulos
   for(unsigned i=1; i<2*n; i++){
      triangulos.push_back({i,i+1,0});
   }
   triangulos.push_back({2*n,1,0});

   // Colores
   col_ver.push_back({1,1,1});
   for(unsigned i = 1; i<vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }

   calcularNormales();  
}

// -----------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'CasaX'

CasaX::CasaX()
:  MallaInd( "CasaX" )
{   
   vertices =
      {  { 0, 0, 0 }, // 0
         { 0, 0, 0.8 }, // 1
         { 0, 0.8, 0 }, // 2
         { 0, 0.8, 0.8 }, // 3
         { +1.0, 0, 0 }, // 4
         { +1.0, 0, 0.8 }, // 5
         { +1.0, 0.8, 0 }, // 6
         { +1.0, 0.8, 0.8 }, // 7
         {  0, 1., 0.4},
         { 1, 1., 0.4}
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {6,2,8},{8,9,6},  // Tejado frontal
         {3,7,8},{8,7,9},  // Tejado trasero

         {2,3,8},{6,7,9},  // Tapas triangulares

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   // Colores
   for(unsigned i = 0; i<vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }
   
}


// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'MallaTriangulo.'

MallaTriangulo::MallaTriangulo()
:  MallaInd( "MallaTriangulo" )
{   
   vertices = {{ 0.5, 0, 0 },  {0,sqrt(2), 0}, {-0.5,0,0}} ;
   triangulos = {{0,1,2}} ;
}


// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'MallaCuadrado'

MallaCuadrado::MallaCuadrado()
:  MallaInd( "MallaCuadrado" )
{   
   vertices =
      {  { -1, -1, 0 }, // 0
         { 1, -1, 0 }, // 1
         { 1, 1, 0 }, // 2
         { -1, 1, 0 }, // 3
      } ;

   triangulos ={{0,1,2}, {0,2,3}} ;   
}


// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'MallaPiramideL'

MallaPiramideL::MallaPiramideL()
:  MallaInd( "MallaPiramideL" )
{   
   vertices ={ {0.5,1,0.5},{0,0,0},{0,0,1},{0.5,0,1},{0.5,0,0.5},{1,0,0.5},{1,0,0}};


   triangulos =
      {  {6,2,1},{4,3,2},{4,6,5}, // base
      } ;

   for(unsigned i=1; i<vertices.size()-1; i++){
      triangulos.push_back({0,i,i+1});
   }
   triangulos.push_back({0,6,1});
}

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'PiramideEstrellaZ'

PiramideEstrellaZ::PiramideEstrellaZ(unsigned n)
:  MallaInd( "PiramideEstrellaZ" )
{
   assert(n>1);
   
   vertices.push_back({0.5,0.5,0});
   for(unsigned i=0; i<2*n; i++){
      if(i%2==0){
         vertices.push_back({0.5+cos(2*PI*i/(2*n))/2, 0.5+sin(2*PI*i/(2*n))/2, 0});
      }else{
         vertices.push_back({0.5+cos(2*PI*i/(2*n))/6, 0.5+sin(2*PI*i/(2*n))/6, 0});
      }
   }
   vertices.push_back({0.5,0.5,0.5});


   // Triángulos
   for(unsigned i=1; i<2*n; i++){
      triangulos.push_back({0,i+1,i});
   }
   triangulos.push_back({2*n,1,0});

   for(unsigned i=1; i<2*n; i++){
      triangulos.push_back({i,i+1,vertices.size()-1});
   }
   triangulos.push_back({2*n,1,vertices.size()-1});

   // Colores
   col_ver.push_back({1,1,1});
   for(unsigned i = 1; i<vertices.size()-1; i++){
      col_ver.push_back(vertices[i]);
   }
   col_ver.push_back({1,1,1});

   
}

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'RejillaY'

RejillaY::RejillaY(unsigned m, unsigned n)
:  MallaInd( "RejillaY" )
{
   assert(m>1 && n>1);
   
   for(unsigned i=0; i<m; i++){
      for(unsigned j=0; j<n; j++){
         vertices.push_back({i*1.0/(m-1), 0, j*1.0/(n-1)});
      }
   }


   // Triángulos
   for(unsigned i=0; i<m-1; i++){
      for(unsigned j=0; j<n-1; j++){
         triangulos.push_back({j+n*i,j+1+n*i,j+n*(i+1)});
         triangulos.push_back({j+n*(i+1), j+n*(i+1)+1, j+1+n*i});
      }
   }

   // Colores
   for(unsigned i = 0; i<vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }

   
}

// -----------------------------------------------------------------------------------------------
// ****************************************************************************
// Clase 'MallaTorre'

MallaTorre::MallaTorre(unsigned n)
:  MallaInd( "MallaTorre" )
{
   
   for(unsigned i=0; i<n+1; i++){
      vertices.push_back({0,i,0});
      vertices.push_back({0,i,1});
      vertices.push_back({1,i,1});
      vertices.push_back({1,i,0});
   }


   // Triángulos
   for(unsigned i=0; i<n; i++){
      for(unsigned j=0; j<3; j++){
         triangulos.push_back({j+i*4,1+j+i*4,j+(i+1)*4});
         triangulos.push_back({j+1+i*4,j+1+(i+1)*4,j+(i+1)*4});
      }
      triangulos.push_back({3+i*4,i*4,3+(i+1)*4});
      triangulos.push_back({i*4,(i+1)*4,3+(i+1)*4});
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
   for(unsigned i=0; i<4; i++){
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
   for(unsigned i=0; i<2; i++){
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
   for(unsigned i=0; i<num_copias-1; i++){
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

// ****************************************************************************
// Clase 'Casa'

Casa::Casa(float h1, float h2, float w, float l)
:  MallaInd( "Casa" )
{
   altura1=h1;
   altura2=h2;
   ancho=w;
   largo=l;


   vertices = {   {-w/2.0,0,0},{-w/2.0,0,l},{w/2.0,0, l},{w/2.0,0,0},
                  {-w/2.0,h1,0},{-w/2.0,h1,l},{w/2.0,h1, l},{w/2.0,h1,0},
                  {0,h2,0},{0,h2,1}    };


   // Triángulos
   triangulos={
                  {0,1,4},{1,5,4},
                  {2,3,6},{3,7,6},
                  {4,5,8},{5,9,8},
                  {6,7,9},{7,8,9}   };

   calcularNormales();

   
   
}



// -----------------------------------------------------------------------------------------------

// ****************************************************************************
// Clase 'Adosadas'

Adosadas::Adosadas(int n, float h1, float h2, float w, float l)
:  MallaInd( "Casa" )
{
   altura1=h1;
   altura2=h2;
   ancho=w;
   largo=l;


   for(int i=0; i<n+1; i++){
      vertices.push_back({-w/2.0,0,i*l});
      vertices.push_back({w/2.0,0,i*l});

      vertices.push_back({-w/2.0,h1,i*l});
      vertices.push_back({w/2.0,h1,i*l});

      vertices.push_back({0,h2,i*l});
   }

   // Triángulos
   for(int i=0; i<n; i++){
      triangulos.push_back({5*i,5*i+5,5*i+2});
      triangulos.push_back({5*i+5,5*i+2,5*i+7});

      triangulos.push_back({5*i+1,5*i+6,5*i+8});
      triangulos.push_back({5*i+8,5*i+1,5*i+3});

      triangulos.push_back({5*i+2,5*i+7,5*i+4});
      triangulos.push_back({5*i+7,5*i+9,5*i+4});
      
      triangulos.push_back({5*i+3,5*i+8,5*i+9});
      triangulos.push_back({5*i+9,5*i+3,5*i+4});
   }
   
}



// -----------------------------------------------------------------------------------------------

// ****************************************************************************
// Clase 'MallaDiscoP4'

MallaDiscoP4::MallaDiscoP4(bool cuadricula){
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31 ;

   for( unsigned i= 0 ; i < ni ; i++ )
      for( unsigned j= 0 ; j < nj ; j++ ){
         const float
            fi = float(i)/float(ni-1),
            fj = float(j)/float(nj-1),
            ai = 2.0*M_PI*fi,
            x  = fj * cos( ai ),
            y  = fj * sin( ai ),
            z  = 0.0 ;
            vertices.push_back({ x, y, z });
      }

   for( unsigned i= 0 ; i < ni-1 ; i++ )
      for( unsigned j= 0 ; j < nj-1 ; j++ ){
         triangulos.push_back({ i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1) });
         triangulos.push_back({ i*nj+j, (i+1)*nj+(j+1), (i+1)*nj+j });
      }


   if(cuadricula){
      for(unsigned i=0; i<ni; i++){
         for(unsigned j=0; j<nj; j++){
            const float
               fi = float(i)/float(ni-1),
               fj = float(j)/float(nj-1),
               ai = 2.0*M_PI*fi,
               x  = fj * cos( ai ),
               y  = fj * sin( ai ),
               z  = 0.0 ;
            cc_tt_ver.push_back({ x/2.0+0.5, y/2.0+0.5 });
         }
      }
   }else{
      for(unsigned i=0; i<ni; i++){
         for(unsigned j=0; j<nj; j++){
            const float
               fi = float(i)/float(ni-1),
               fj = float(j)/float(nj-1);

            cc_tt_ver.push_back({ fi,fj});
         }
      }
   }
   
}

