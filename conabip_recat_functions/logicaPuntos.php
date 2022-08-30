<?php 
/*Ajustar segun corresponda e incluir el siguiente texto en el confirmaton meesage de webform y en aditonal settings*/

print_r($form_values); die();

//Puntos Acervo Bibliografico
$puntos[7]['Hasta 1500']=0;
$puntos[7]['De 1501 a 5000']=1;
$puntos[7]['De 5001 a 10000']=2;
$puntos[7]['De 10001 a 15000']=4;
$puntos[7]['De 15001 a 20000']=6;
$puntos[7]['Más de 20000']=8;

//Puntos Mat Audiovisual
$puntos[8]['Hasta 100']=0;
$puntos[8]['De 101 a 500']=1;
$puntos[8]['Más de 500']=3;

//Puntos Prestamos en sala
$puntos[9]['Hasta 3600']=0;
$puntos[9]['De 3601 a 7200']=1;
$puntos[9]['De 7201 a 12000']=2;
$puntos[9]['De 12001 a 18000']=3;
$puntos[9]['Más de 18000']=4;

//Puntos Prestamos a domicilio
$puntos[10]['Hasta 3600']=0;
$puntos[10]['De 3601 a 6000']=1;
$puntos[10]['De 6001 a 8400']=2;
$puntos[10]['De 8401 a 12000']=3;
$puntos[10]['Más de 12000']=4;

//Puntos Prop y prom cultural
$puntos[11]['Hemeroteca']=2;
$puntos[11]['Acceso a PC para usuarios']=1;
$puntos[11]['Acceso a Internet para usuarios']=1;
$puntos[11]['Capacitación en TICs']=2;
$puntos[11]['Eventos Artísticos']=2;
$puntos[11]['Actividades Regulares de Animación a la lectura en niños y niñas']=1;
$puntos[11]['Actividades Regulares de Animación a la lectura en Adolescentes y Jóvenes']=1;
$puntos[11]['Actividades Regulares de Animación a la lectura en Adultos']=1;
$puntos[11]['Actividades Regulares de Animación a la lectura en Adultos Mayores']=1;
$puntos[11]['Animación a la Lectura fuera de la biblioteca']=2;
$puntos[11]['Promoción de Derechos (Información Ciudadana sobre Derechos, Servicios y Trámites)']=3;
$puntos[11]['Servicios para Discapacidad']=2;
$puntos[11]['Servicios Móviles (bibliomóvil, bibliobici, caja viajera, etc.)']=2;
$puntos[11]['Filiales']=1;
$puntos[11]['Publicaciones Impresas con frecuencia mínima mensual (Boletines o Revistas)']=1;

//Puntos Socios con cuota al dia 
$puntos[12]['Hasta 30']=0;
$puntos[12]['De 31 a 100']=4;
$puntos[12]['De 101 a 200']=5; 
$puntos[12]['De 201 a 300']=6; 
$puntos[12]['De 301 a 400']=7; 
$puntos[12]['De 401 a 500']=8; 
$puntos[12]['Más de 500']=9;

//Puntos usuarios anuales 
$puntos[13]['Hasta 4800']=1;
$puntos[13]['De 4801 a 8400']=2;
$puntos[13]['De 8401 a 12000']=3;
$puntos[13]['De 12001 a 16000']=4;
$puntos[13]['Más de 16000']=5;

//Puntos Personal Rentado 
$puntos[16]['Ninguno']=0;
$puntos[16]['De 1 a 2']=3;
$puntos[16]['De 3 a 5']=4;
$puntos[16]['Más de 5']=5;

//Puntos Personal Universitario 
$puntos[15]['1 Bibliotecario con nivel universitario']=4;
$puntos[15]['Más de 1 Bibliotecario con nivel universitario']=5;

//Puntos Personal Terciario
$puntos[17]['1 Bibliotecario con nivel terciario']=3;
$puntos[17]['Más de 1 Bibliotecario con nivel terciario']=4;

//Puntos Personal Idoneo
$puntos[18]['Bibliotecario idóneo']=2;
$puntos[18]['Más de 1 Bibliotecario idóneo']=3;

//Puntos Personal Docente
$puntos[19]['1 Personal docente']=1;
$puntos[19]['Más de 1 Personal docente']=2;

//Puntos voluntarios
$puntos[20]['Ninguno']=0;
$puntos[20]['De 1 a 2']=3;
$puntos[20]['De 3 a 5']=4;
$puntos[20]['Más de 5']=5;

//Puntos Hs Semanales
$puntos[22]['De 20 a 30 hs']=4;
$puntos[22]['De 31 a 40 hs']=5;
$puntos[22]['Más de 40 hs']=6;

//Puntos fines de semana
$puntos[23]['Sábado o Domingo']=1;
$puntos[23]['Sábado y Domingo']=2;

//Puntos DIGIBEPE
$puntos[25]['Ya incorporaron Digibepé']=5;
$puntos[25]['No incorporaron Digibepé']=0;

//Puntos Nivel procesamiento
$puntos[26]['Obras Procesadas hasta el 25%']=1;
$puntos[26]['Obras Procesadas hasta el 50%']=2;
$puntos[26]['Obras Procesadas hasta el 75%']=3;
$puntos[26]['Obras Procesadas hasta el 100%']=4;


//Puntos sup. edificio
$puntos[28]['Hasta 50 m2']=1;
$puntos[28]['De 51 m2 a 100 m2']=2;
$puntos[28]['De 101 m2 a 200 m2']=3;
$puntos[28]['Más de 200 m2']=4;

//Puntos cantidad de instalaciones
$puntos[29]['Sala de Lectura']=0;
$puntos[29]['Sala de Lectura+Sector Infantil']=2;
$puntos[29]['Sala deLectura+Sector Infantil+SUM']=3;

//Puntos Antiguedad
$puntos[31]['Hasta 15 años']=0;
$puntos[31]['De 16 a 50 años']=1;
$puntos[31]['De 51 a 99 años']=2;
$puntos[31]['Más de 100 años']=3;



$cids=array_keys($form_values['submitted']);



foreach($cids as $key){
	if($key!=11){
		$suma= $suma + $puntos[$key][$form_values['submitted'][$key]];
	}
	if($key==11){
		$key2=0;
		foreach ($form_values['submitted'][$key] as $multiplefield){
			$suma= $suma + $puntos[$key][$form_values['submitted'][$key][$key2]];
			$key2++;
		}
	}
 
}

$categoriaResultante="C";
if(($suma/20)>=2.6){$categoriaResultante="B";}
if(($suma/20)>=4){$categoriaResultante="A";}


$form_values['submitted'][32]= $suma;
$form_values['submitted'][33]=$categoriaResultante;
$categoriaAsignada= cnbp_obtener_categoria($categoriaResultante);
$form_values['submitted'][34]=$categoriaAsignada;

?>
Ha finalizado el trámite de recategorización. La categoría asignada es <b><?php echo $categoriaAsignada; ?></b>.
