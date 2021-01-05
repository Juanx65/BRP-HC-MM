#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <list>	
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <algorithm>

std::vector< std::vector<int>> full_random_gen(int col,int tier,int maxValueOnFile,std::vector< std::vector<int>>  bahia,int movN);
void MMgenerator(std::vector<int>::iterator , std::vector<std::vector<int>>,std::vector<std::vector<int>>&,int, int, int);
void addRandSol (int,int,int,int,int,std::vector<std::vector<int>>&,std::vector<std::vector<int>>&);
void actualizadorBahia(int, std::vector<std::vector<int>>, std::vector<std::vector<int>>&,int);
std::vector<std::vector<int>> MovShiftCol(int,int, std::vector<std::vector<int>>, std::vector<std::vector<int>>);
void MMShiftCol(int numMov,int tier,int col, std::vector<int> colV, std::vector<std::vector<int>>& solOp, std::vector<std::vector<int>> bahia);
void printNestedList(std::vector<std::vector<int>> );
void gen_random_sol(int,int,int,std::vector< std::vector<int>>&,std::vector< std::vector<int>>,int&, int);
int getRandomFac(int,int, std::vector<std::vector<int>> );

int main(int argc, char* argv[])
{
	srand(time(0));// para generar nueros aleatorios.

//-----------------------------command line handler inputs error--------------------------------------------//
	if(argc != 2)
	{
		std::cout << "Numero de argumentos Erroneo" << std::endl;
		return -1;
	}
	std::string fn = argv[1];
	if (fn == "-help")
	{
		std::cout << "Ingrese el archivo de configuracion deseado..." << std::endl;
		return 0;
	}
	else if(fn.substr(fn.find_last_of(".") + 1) != "txt")
	{
		std::cout << "Error en el tipo de archivo" << std::endl;
		return -1;
	}
//-------------------------------------------File Handler---------------------------------------------------//

	std::string word;							// leeremos le archivo palabra por palabra
	std::fstream configFile(fn);				//abrimos el archivo
	int cont = 0, ccol = 0, ctier = 0;			//contadores
	int col, tier;								//buffers para los parametros col y tier.
	std::vector< std::vector<int> > bahia;		/*vector de vectores usado para guardar bahia[columna][tier]
												  que corresponde a la informacion guardada en el archivo de texto	*/
	std::vector<int> columna;					//vector tipo buffer para ordenar la información 		
	int cvalue;									//otro contador
	int maxValueOnFile = 0;						// guarda el mayor valor entre los containers.
	while (configFile >> word) 					
	{
  		if(cont == 1)
		{
			std::stringstream scol(word);
			scol >> col;
			//std::cout << "columans: "<<col << std::endl;
		}
		else if (cont == 3)
		{
			std::stringstream stier(word);
			stier >> tier;
			//std::cout << "tiers: "<< tier << std::endl;
		}
		else if (cont > 3)
		{
			//rellenar el vector de vectores
			// para tener bahia[columna][tier]
			// donde bahia[0][0] corresponde al container ubicando en la cima de la primera columna
			if (ctier < tier && ccol < col)
			{	
				std::stringstream value(word);
				value >> cvalue;
				if(cvalue > maxValueOnFile)
							maxValueOnFile = cvalue;
				columna.push_back(cvalue);
				ccol ++;
			}
			if(ccol >= col)
			{	
				std::vector<int>::iterator itr;
				std::vector<int> var;
				if(bahia.empty())
				{
					for (itr = columna.begin(); itr != columna.end();itr++)
					{
						var.push_back(*itr);
						bahia.push_back(var);
						var.erase(var.begin(), var.end()); 
					}
				}
				else
				{
					int ccont=0;
					for (itr = columna.begin(); itr != columna.end();itr++)
					{
						bahia[ccont].push_back(*itr);
						ccont ++;
					}
				}
				columna.erase(columna.begin(), columna.end()); 
				ccol = 0;
				ctier ++;
			}
			
		}
		cont ++;
  		//std::cout << word <<std::endl;	
	}
	// Close the file
	//std::cout << bahia[0][3] << std::endl;
	configFile.close(); 

//-------------------------------- Funcion principal --------------------------------------------------------//
	
	int movN = 0; // para mantener la cantidad de movimientos ( no es realmente necesario necesario ).
	std::vector<std::vector<int>> sol_in,solOp,sol, MMsol; // vectores para manejar la solucion inicial, de la optima y la de MM.
	// generacion de solucion inicial factible de manera aleatoria 
	//gen_random_sol(col,tier,maxValueOnFile,sol_in,bahia,movN,1);

	sol_in = full_random_gen(col,tier,maxValueOnFile,bahia,movN); // aqui generamos la solucion inicial aleatoria.

	movN = sol_in.size(); // la cantidad de movimeintos de la solucion inicial.

	//std::cout << "movimientos solucion Random inicial: " << movN << std::endl;
	//std::cout << "Solucion inicial: " << std::endl;
	//printNestedList(sol_in);

	/* INICIALIZACION DE VARIABLES */
	sol = sol_in;
	solOp = sol_in;
	MMsol = sol_in;

	int explorador = 0;

	int ITERACIONES = 500;
	int EXPLOTACIONES = 50;

	/* Loop donde generamos realizamos 'ITERACIONES' iteraciones, en las cuales se generaran 'EXPLOTACIONES' soluciones explotando cada solucion
	   random inicial, por lo tanto se generaran 'ITERACIONES'/'EXPLOTACIONES' exploraciones en total */
	for(int iterator = 0 ; iterator < ITERACIONES ; iterator++) 
	{	
		if(explorador == EXPLOTACIONES)
		{	
			sol = full_random_gen(col,tier,maxValueOnFile,bahia, 0 ); // Se genera una nueva solucion random para explorar.
			//std::cout << "Random sol mov = " << sol.size() << std::endl;
			explorador = 0;
		}
		explorador++;
	
		solOp = MovShiftCol(col,tier,sol,bahia); /* se busca la solucion con MM dada una solucion inicial y se sigue iterando 
													su mejor mejora durante'EXPLOTACIONES' veces.*/
		if(solOp.size() < sol.size())
			sol = solOp;
		if(sol.size() < MMsol.size())
			MMsol = sol;
		//sol = MMsol;
		//std::cout << "Iteracion: "<< iterator << " MM = " << sol.size() << std::endl;
	}

	std::cout << "Solución encontrada con HC MM: " << std::endl;
	printNestedList(MMsol);
	std::cout << "Total de movimientos con HC MM: " << MMsol.size() << std::endl;

	return 0;
}

/*
	funcion usada para generar la solucion inicial factible y las soluciones de exploracion, realmente no es necesaria, ya que 
	usa gen_random_sol, fue creada para mejor simplisidad en su aplicacion.
*/
std::vector< std::vector<int>> full_random_gen(int col,int tier,int maxValueOnFile,std::vector< std::vector<int>>  bahia,int movN)
{
	std::vector< std::vector<int>> RanSol;
	gen_random_sol(col,tier,maxValueOnFile,RanSol,bahia,movN,1);
	return RanSol;
}

/*
	generamos una solucion aleatoria factible ( ya sea para iniciar o para explorar, y para reparar soluciones luedo del shift)
*/
void gen_random_sol(int col,int tier, int max, std::vector< std::vector<int>>& rand_sol,std::vector< std::vector<int>> bahia ,int& movN, int i)
{
	int col_sel = 0, tier_sel = 0;
	bool ifbreak = false;
	// encontramos la ubicacion exacta y en orden del container a extraer
	//std::cout << "Distribucion inicial con bahia[0][0] esquina superior izquierda" << std::endl;
	//printNestedList(bahia);
	for (i ; i <= max; i++)
	{	
		for (int ccol = 0; ccol < col; ccol++)
		{	
			for (int ctier = 0; ctier < tier; ctier++)
			{
				if(bahia[ccol][ctier] == i)
				{
					ifbreak = true;
					col_sel = ccol; 
					tier_sel = ctier;

					// En el caso de que el container a retirar esta en la cima del tier, lo retiramos inmediatamente
					if(ctier == 0)
					{
						//std::cout << "Container " << i << " fue retirado" << std::endl;
						bahia[ccol][ctier] = 0;
						rand_sol.push_back({i,ccol,ctier,-1,-1}); // agregamso el retro al vector, con retiro como [-1][-1]
						//std::cout << "movimeinto agregado "<<"{" << i << ", "<< ccol <<", " <<ctier <<", " << -1 << ", " << -1 << "}" << std::endl;
						movN ++;
					}
					// si el container a relocalizar no se encuentra en la cima
					else if (ctier > 0 )
					{
						int czero = 0;
						int cnonzero = 0;
						int reductor = 0;
						for (int j = ctier - 1; j >= 0; --j)
						{
							// si solo hay espacios vacios sobre el container a relocalizar, lo retiramos de inmediato
							if(bahia[ccol][j] == 0)
							{
								czero ++;
								if (czero == ctier)
								{
									//std::cout << "Container " << i << " fue retirado" << std::endl;
									rand_sol.push_back({i,ccol,ctier,-1,-1}); // agregamso el retro al vector, con retiro como [-1][-1]
									//std::cout << "movimeinto agregado "<<"{" << i << ", "<< ccol <<", " <<ctier <<", " << -1 << ", " << -1 << "}" << std::endl;
									movN ++;
									bahia[ccol][ctier] = 0;
								}
								
							}
							/* 
							debemos realocar el valor inmediatamente arriba, lo hacemos al azar
							si el container inmediatamente arriba, tiene otro container inmediatamente arriba, debemos relocalizar ese primero
							*/
							else 
							{	
								if(j != 0 && bahia[ccol][j-1] != 0)
								{
									//j siguiente  ( es decir, evaluamos el siguiente container)
								}
								else
								{
									if(reductor == 0)
										i--;
									reductor ++;
									addRandSol (movN,col,tier,ccol,j,bahia,rand_sol);
									movN ++;
									//i --; // debemos volver a intentar sacar el container i							
								}	
							}
						}
					}
					break;	
				}
			}
			if(ifbreak)
				break;
		}
		ifbreak = false;
		//printNestedList(rand_sol);
	}
	//printNestedList(rand_sol);
	return;
}

/*
	mediante el vector de movimientos de sol y la bahia inicial, actualiza la bahia
	hasta el movimienot entregado, permite situarce en la bahia en un movimeinto especifico.
*/
void actualizadorBahia(int mov, std::vector<std::vector<int>> sol, std::vector<std::vector<int>>& bahia,int i=0 )
{
	//std::cout << "Actualizador de bahia movimiento: " << mov << std::endl;
	for(i; i <= mov; i++)
	{
		if( sol[i][3] == -1 )
		{
			bahia[ sol[i][1] ][ sol[i][2] ] = 0;
		}
		else
		{
			bahia[ sol[i][3] ][ sol[i][4] ] = bahia[ sol[i][1] ][ sol[i][2] ];
			bahia[ sol[i][1] ][ sol[i][2] ] = 0;
		}
		//printNestedList(bahia);
	}
	//printNestedList(bahia);
	return;
}

/*
	funcion que de todos los switch efectuados entre columnas permitidas, escojera el mejor cambio
	retornara la lista de movimientos de containers actualizada a la mejor mejora encontrada.
*/
std::vector<std::vector<int>> MovShiftCol(int col,int tier ,std::vector<std::vector<int>> solin, std::vector<std::vector<int>> bahia)
{
	std::vector<std::vector<int>> sol = solin; // se ira guardando la mejor solucion
	std::vector<std::vector<int>> sol_in = solin; 
	std::vector<std::vector<int>> solOp = solin;
	std::vector<std::vector<int>> bahia_in;
	bahia_in = bahia;
	int col_llegada=0;
	int col_retiro=0;
	std::vector<int> col_candidata;

	for(int i = 0; i < sol.size(); i++) // antes movNin
	{
		if( sol[i][3] != -1 ) // si se trata de una relocalizacion , cambiamos la columna de destio, si es factible 
		{
			col_llegada = sol[i][3];
			col_retiro = sol[i][1];
			
			for(int ccol=(col-1); ccol >= 0 ; ccol--) // buscar columnas que tengas espacio para relocalizar
				if(ccol != col_llegada && ccol != col_retiro)
					if(bahia[ccol][0] == 0)
					{
						//std::cout << "Se añade la columna: " << ccol <<". A columnas posibles, movimieto :"<< i <<std::endl;
						col_candidata.push_back(ccol); // vector de columnas candidatas para hacer shift
					}
			if(!col_candidata.empty())
			{
				MMShiftCol(i,tier,col,col_candidata,sol,bahia);
				if(sol.size() < solOp.size())
					solOp = sol;
			}
		}	
		bahia = bahia_in;
		sol = sol_in;
		col_candidata.clear();
		actualizadorBahia(i,sol,bahia);// volvemos desde el punto inicial de la solucion random para un nuevo shift
	}
	return solOp;
}

/*
	para una instancia de solucion y un cambio en de columna, reviza cual seria la cantidad de movimientos
	optima y modificamos la solucion para obtener la solcion optima, es usada dentro de MovShiftCol 
*/
void MMShiftCol(int numMov,int tier,int col, std::vector<int> colV, std::vector<std::vector<int>>& solOp, std::vector<std::vector<int>> bahia)
{
	std::vector<std::vector<int>> sol = solOp;
	std::vector<std::vector<int>> sol_in = solOp;
	std::vector<int>::iterator ptr;

	//std::cout << "MMShift mov original: bahia[" << sol[numMov][1]<<"]["<<sol[numMov][2]<<"] -> bahia[" << sol[numMov][3]<<"]["<<sol[numMov][4]<<"] mov: "<< numMov << std::endl;
	//printNestedList(bahia);
	
	if(colV.size() > 1) //iteramos por cada columna posible para buscar el mejor movimiento
	{
		std::sort(colV.begin(), colV.end()); 
		for (ptr = colV.begin() ;ptr < colV.end(); ptr++)
		{	
			MMgenerator(ptr,bahia,sol,numMov,tier,col);
			if(sol.size() < solOp.size())
					solOp = sol;
			sol = sol_in;
		}

	}
	else
	{
		MMgenerator(colV.begin(),bahia,sol,numMov,tier,col);
		if(sol.size() < solOp.size())
			solOp = sol;
	}
	
	//std::cout << "SOL FINAL con ";
	//std::cout << "NUM MOV: " << solOp.size() <<std::endl;
	//printNestedList(solOp);
	return;
}

/*
	busca y arregla las soluciones posibles para generar la MM, Es usada dentro de MMShiftCol
*/
void MMgenerator(std::vector<int>::iterator ptr, std::vector<std::vector<int>> bahia,std::vector<std::vector<int>>& sol,int numMov, int tier, int col )
{
	int movN = 0;
	int max = 0;
	int mov_freez = 1;
	//std::cout << "Shift: bahia[" <<sol[numMov][3]<<"]["<<sol[numMov][4]<<"] por ";
		
		sol[numMov][3] = *ptr; // cambiamos la columna de llegada
		for (int rtier = (tier-1); rtier >= 0; rtier--)		
			if(bahia[*ptr][rtier] == 0)
			{
				sol[numMov][4] = rtier;	
				break;	
			}

		//std::cout << " bahia[" << sol[numMov][3] << "][" <<sol[numMov][4] << "]" << ". movimeinto: " << numMov << std::endl;
		bahia[ sol[numMov][3] ][ sol[numMov][4] ] = bahia[ sol[numMov][1] ][ sol[numMov][2] ];
		bahia[ sol[numMov][1] ][ sol[numMov][2] ] = 0;

		for(int i = 0; i < sol.size();i++ ) 
			if(sol[i][0] > max)
				max = sol[i][0];

		for(int i = numMov+1; i < sol.size(); ) // eliminamos los mov futuros q nacieran a partir de la sol modificada
			sol.erase(sol.begin() + i);

		for(int i = 0; i < sol.size();i++ ) //buscamos en q mov quedamos		
			if(sol[i][3] == -1)
				if(sol[i][0] > mov_freez)
					mov_freez = sol[i][0];

		//std::cout << " max= " << max << std::endl;
		//std::cout << " mov_freez= " << mov_freez << std::endl;

		movN = sol.size();
		// agrregamos nuevas soluciones completamente alazar a partir del cambio XD
		gen_random_sol(col,tier,max,sol,bahia,movN,mov_freez + 1);
		
		//printNestedList(sol);
		//std::cout << " numero de movimeintos= " << movN << std::endl;
		//std::cout << "__________________________________________________________________________________"<<std::endl;
	return;
}

/*
	añade un movimeito factible al azar a la lista de movimientos y actualiza la bahia con dicho movimiento 
*/
void addRandSol (int mov, int col,int tier, int ccol, int ctier,std::vector<std::vector<int>>& bahia,std::vector<std::vector<int>>& sol)
{
	int crtier = 0;
	int rando = getRandomFac(col, ccol, bahia);
	for (int rtier = (tier-1); rtier >= 0; rtier--)
		if(bahia[rando][rtier] == 0)
		{
			crtier = rtier;
			break;
		}
	//std::cout << "addRandSol -> bahia "<<"[" << rando << "][" << crtier << "] " << "recibe container " << bahia[ccol][ctier] << std::endl;
	sol.insert((sol.begin()+mov),{bahia[ccol][ctier],ccol,ctier,rando,crtier}); // agregamos el mov a la lista de sol
	bahia[rando][crtier] = bahia[ccol][ctier];
	bahia[ccol][ctier] = 0;
	//std::cout << "Sol: "<<std::endl;
	//printNestedList(sol);
	//std::cout << "movimeinto agregado "<<"{" << ccol <<", " <<j <<", " << rando << ", " << crtier << "}" << std::endl;
	return;
}

/*
	genera un movimiento de columna factible la fun gen_random_sol
*/
int getRandomFac(int col, int ccol, std::vector<std::vector<int>> bahia)
{
	int rando = 0;
	//srand(time(0));
	while(true)
	{
		rando=(rand() % col);
		if(rando == ccol)
		{
			if(rando == col || rando == (col - 1))
			{
				rando --;
			}
			else
			{ 
				rando ++;
			}
		}
		//std::cout << "columna receptora generada aleatoriamente: "<< rando << std::endl;
		if(bahia[rando][0] == 0)
		{
			return rando;
		}
		else
		{
			if (rando == col || rando == (col - 1))
			{
				rando --;
			}
			else				
			{
				rando ++;
			}
		}
	}
}

/*
	funcion que imprime un vector de vectores de int por pantalla, funcion utilizada para visualizar resultados de solucion
	y el avance de la bahia.
*/
void printNestedList(std::vector<std::vector<int>> nested_list) 
{ 
    std::cout << "[\n"; 
  
    // nested_list`s iterator(same type as nested_list) 
    // to iterate the nested_list 
    std::vector<std::vector<int>>::iterator nested_list_itr; 
  
    // Print the nested_list 
    for (nested_list_itr = nested_list.begin(); 
         nested_list_itr != nested_list.end(); 
         ++nested_list_itr) { 
  
        std::cout << "  ["; 
  
        // normal_list`s iterator(same type as temp_list) 
        // to iterate the normal_list 
        std::vector<int>::iterator single_list_itr; 
  
        // pointer of each list one by one in nested list 
        // as loop goes on 
        std::vector<int>& single_list_pointer = *nested_list_itr; 
  
        for (single_list_itr = single_list_pointer.begin(); 
             single_list_itr != single_list_pointer.end(); 
             single_list_itr++) { 
            std::cout << " " << *single_list_itr << " "; 
        } 
        std::cout << "]\n"; 
    } 
    std::cout << "]" << std::endl; 
} 