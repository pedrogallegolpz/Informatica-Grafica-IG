// Nombre: PEDRO, Apellidos: GALLEGO LÓPEZ, (IG GIM+GIADE, curso 21-22)

// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
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


#include "matrices-tr.h"
#include "materiales-luces.h"
#include <cstring>

using namespace std ;

const bool trazam = false ;

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   // COMPLETADO: práctica 4: cargar imagen de textura
   // (las variables de instancia están inicializadas en la decl. de la clase)
   // .....
   imagen = LeerArchivoJPEG(nombreArchivoJPG.c_str(), ancho, alto);
}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETADO: práctica 4: enviar la imagen de textura a la GPU
   // y configurar parámetros de la textura (glTexParameter)
   // .......
   glGenTextures(1, &ident_textura);                    // genera identificador
   glBindTexture( GL_TEXTURE_2D, ident_textura );       // activa textura con identificador ’idTex’ :
   
   // Envía a la GPU
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, ancho, alto, GL_RGB, GL_UNSIGNED_BYTE, imagen);

   enviada = true;
}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != nullptr )
      delete [] imagen ;

   imagen = nullptr ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar( Cauce & cauce  )
{
   // COMPLETADO: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......

   if(!enviada){
      enviar();
   }
   cauce.fijarEvalText(enviada, ident_textura);
   cauce.fijarTipoGCT(modo_gen_ct, coefs_s, coefs_t);
}
// *********************************************************************

TexturaXY::TexturaXY( const std::string & nom ):Textura(nom){
   //Práctica 4: implementar constructor de Textura XY
   modo_gen_ct = mgct_coords_objeto;
   coefs_s[0]=1.0;
   coefs_s[1]=0.0;
   coefs_s[2]=0.0;
   coefs_s[3]=0.0;

   coefs_t[0]=0.0;
   coefs_t[1]=1.0;
   coefs_t[2]=0.0;
   coefs_t[3]=0.0;
   
}// *********************************************************************

TexturaXZ::TexturaXZ( const std::string & nom ):Textura(nom){
   //Práctica 4: implementar constructor de Textura XZ
   modo_gen_ct = mgct_coords_objeto;
   coefs_s[0]=1.0;
   coefs_s[1]=0.0;
   coefs_s[2]=0.0;
   coefs_s[3]=0.0;

   coefs_t[0]=0.0;
   coefs_t[1]=0.0;
   coefs_t[2]=1.0;
   coefs_t[3]=0.0;
}

// *********************************************************************
// crea un material usando un color plano y los coeficientes de las componentes

Material::Material( const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = nullptr ;
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ;
}

//----------------------------------------------------------------------
// crea un material usando una textura y los coeficientes de las componentes

Material::Material( Textura * p_textura, const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = p_textura ;  assert( textura != nullptr );
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( textura != nullptr )
   {  delete textura ;
      textura = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar( ContextoVis & cv )
{
   // COMPLETADO: práctica 4: activar un material
   // .....
   
   if(textura!= nullptr){
      textura->activar(*cv.cauce_act);

      assert(exp_pse>=1);
   }else{
      cv.cauce_act->fijarEvalText(false);    // Necesario para desactivar texturas previas en objetos actuales sin textura
   }
   Tupla3f mil_ka =Tupla3f(k_amb,k_amb,k_amb);
   Tupla3f mil_kd = Tupla3f(k_dif,k_dif,k_dif);
   Tupla3f mil_ks =Tupla3f(k_pse,k_pse,k_pse);
   cv.cauce_act->fijarParamsMIL(mil_ka, mil_kd, mil_ks, exp_pse);
   //cv.cauce_act->fijarEvalMIL(true); // Esto no se si hay que ponerlo

   // registrar el material actual en el cauce
   cv.material_act = this ; 
   
}
//**********************************************************************

FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const Tupla3f & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi     = longi_ini ;
   lati      = lati_ini ;
   color     = p_color ;

   // col_ambiente  = p_color ;
   // col_difuso    = p_color ;
   // col_especular = p_color ;
   //ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice
   //CError();
}

//----------------------------------------------------------------------
// para fuentes diraccionales, incrementar o decrementar la longitud
// (en las puntuales no hace nada)
void FuenteLuz::actualizarLongi( const float incre )
{
   longi = longi + incre ;
   using namespace std ;
   cout << "actualizado angulo de 'longitud' de una fuente de luz, nuevo == " << longi << endl ;
}
//----------------------------------------------------------------------
// para fuentes diraccionales, incrementar o decrementar la longitud
// (en las puntuales no hace nada)
void FuenteLuz::actualizarLati( const float incre )
{
   lati = lati + incre ;
   using namespace std ;
   cout << "actualizado angulo de 'latitud' de una fuente de luz, nuevo == " << lati << endl ;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   //pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( Cauce & cauce )
{
   // COMPLETADO: práctica 4: activar una colección de fuentes de luz
   //   (crear un array con los colores y otro con las posiciones/direcciones,
   //    usar el cauce para activarlas)
   // .....
   vector<Tupla3f> color;
   vector<Tupla4f> pos_dir;
   Tupla4f pdir;

   for(unsigned i=0; i<vpf.size(); i++){
      color.push_back(vpf[i]->color);
      
      float coord1 = cos(vpf[i]->longi*M_PI/180.0)*cos(vpf[i]->lati*M_PI/180);
      float coord2 = sin(vpf[i]->lati*M_PI/180.0);
      float coord3 = sin(vpf[i]->longi*M_PI/180.0)*cos(vpf[i]->lati*M_PI/180);
      float coord4 = 0.0;

      pdir={coord1, coord2, coord3, coord4};
      pos_dir.push_back(pdir.normalized());
   }
   cauce.fijarFuentesLuz(color, pos_dir);
}

// ---------------------------------------------------------------------
// pasa a la siguiente fuente de luz (si d==+1, o a la anterior (si d==-1))
// aborta si 'd' no es -1 o +1

void ColFuentesLuz::sigAntFuente( int d )
{
   assert( i_fuente_actual < vpf.size()) ;
   assert( d == 1 || d== -1 );
   i_fuente_actual = unsigned((int(i_fuente_actual+vpf.size())+d) % vpf.size()) ;
   cout << "fuente actual: " << (i_fuente_actual+1) << " / " << vpf.size() << endl ;
}

// ---------------------------------------------------------------------
// devuelve un puntero a la fuente de luz actual

FuenteLuz * ColFuentesLuz::fuenteLuzActual()
{
   assert( vpf[i_fuente_actual] != nullptr );
   return vpf[i_fuente_actual] ;
}
//----------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}

//--------------------------------------------------------------------------
// actualizar una colección de fuentes en función de una tecla GLFW pulsada
// (se usa el código glfw de la tecla, se llama desde 'main.cpp' con L pulsada)
// devuelve 'true' sii se ha actualizado algo

bool ProcesaTeclaFuenteLuz( ColFuentesLuz * col_fuentes, int glfw_key )
{
   assert( col_fuentes != nullptr );

   FuenteLuz * fuente     = col_fuentes->fuenteLuzActual() ; assert( fuente != nullptr );
   bool        redib      = true ;
   const float delta_grad = 2.0 ; // incremento en grados para long. y lati.

   switch( glfw_key )
   {
      case GLFW_KEY_RIGHT_BRACKET : // tecla '+' en el teclado normal
         col_fuentes->sigAntFuente( +1 );
         break ;
      case GLFW_KEY_KP_ADD :
         col_fuentes->sigAntFuente( +1 );
         break ;
      case GLFW_KEY_SLASH :        // tecla con '-' y '_' en el teclado normal
      case GLFW_KEY_KP_SUBTRACT :
         col_fuentes->sigAntFuente( -1 );
         break ;
      case GLFW_KEY_LEFT :
         fuente->actualizarLongi( +delta_grad );
         break ;
      case GLFW_KEY_RIGHT :
         fuente->actualizarLongi( -delta_grad );
         break ;
      case GLFW_KEY_DOWN :
         fuente->actualizarLati( -delta_grad );
         break ;
      case GLFW_KEY_UP :
         fuente->actualizarLati( +delta_grad );
         break ;
      default :
         redib = false ;
         break ;
   }
   return redib ;
}

//-----------------------------------------------------------------------
// constructor de una colección de fuentes de luz sencilla que incluye
// dos fuentes de luz.

Col2Fuentes::Col2Fuentes()
{
   const float f0 = 0.7, f1 = 0.3 ;
   insertar( new FuenteLuz( +45.0, 60.0,  Tupla3f { f0, f0,     f0,    } ) );
   insertar( new FuenteLuz( -70.0, -30.0, Tupla3f { f1, f1*0.5, f1*0.5 } ) );

}
