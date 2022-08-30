<?php
function rec_by_id($rec_id)
{

  $host="cc.bepe.ar:7090/biblios";
  $id = yaz_connect($host);
  yaz_syntax($id, "usmarc");
  yaz_sort($id, "1=1003 ia 1=4 ia");
  $finalquery = $rec_id;
  yaz_search($id, "rpn", $finalquery);
  yaz_wait();
  $error = yaz_error($id);
  if (!empty($error)) {
    echo "Error: $error";
  } else {
    $hits = yaz_hits($id);
            //echo "Result Count $hits";
  }
  $rec = yaz_record($id, 1, "xml");
  return $rec;
}



function forma_item($form_state)
{

  $bib_id = $_GET['bib_id']; 
  $rec = rec_by_id($bib_id); 
//print_r($rec); //rec
  $param_bibs  = $_GET['bibs'];
  $bibs = explode('/',$param_bibs);




  $nombre_bibs = array('data' => array());
  for ($i=0;$i < count($bibs) - 1;$i++)
   { if (($bibs[$i]) && ($bibs[$i] != 'no'))
 $nombre_bibs[]=array(nombrebp($bibs[$i]));
}
$header = array('Bibliotecas');
//$tabla_bibs =  theme_table($header, $nombre_bibs);
$tabla_bibs = theme_table(array('header'=>array('data'=>$header),'rows'=>array('data'=>$nombre_bibs),'attributes' => array('width' => '100%')));
$res = xmltoarray($rec);
$index = $res['index'];
$vals  = $res['vals'];
$autor = getvaluebytag($index, $vals, '100', 'a');
$titulo = getvaluebytag($index, $vals, '245', 'a');
$editorial = getvaluebytag($index, $vals, '260', 'b');
$anio = getvaluebytag($index, $vals, '260', 'c');
$lugar = getvaluebytag($index, $vals, '260', 'a');
$isbn = getvaluebytag($index, $vals, '20', 'a');
$form['titulo'] = array('#type' => 'item',
  '#value' => $titulo,
  );

if ($autor)
 $form['autor'] = array('#type' => 'item',
  '#value' => 'Por '.$autor,
  );

$tlugar='';
if ($lugar) $tlugar = '('.$lugar.")";
if ($editorial)
 $form['editorial'] = array('#type' => 'item',
  '#value' => 'Publicado por '.$editorial.$tlugar,
  );
if ($anio)
 $form['anio'] = array('#type' => 'item',
  '#value' => 'A&ntilde;o:'.$anio,
  );
if ($isbn)
  $form['isbn'] = array('#type' => 'item',
    '#value' => 'ISBN:'.$isbn,
    );

$form['mensaje_de_error'] = array('#type' => 'item',
  '#value' => $mensaje_de_error,
  );
$form['bibs'] = array('#type' => 'item',
  '#value' => $tabla_bibs,
  );

return $form;
}

/*
*
CONSULTA Y RESULTADOS 
Esta función muestra el form y si recibe la consulta. Si hay resultados los muestra dentro del form como un elemento HTML.
*/
function forma_consulta($form_state)
{



  $bib_id = $_GET['bib_id']; 
  $debug = $_GET['debug'];


  session_start();
  $vars_forma_consulta=$_SESSION['vars_forma_consulta'];
//global $vars_forma_consulta;




  if ($vars_forma_consulta[start] < 1) {$vars_forma_consulta[start]=1;}
  if ( ! isset($vars_forma_consulta[hits])  ) {$vars_forma_consulta[hits]=0;}
  if (($bib_id) )
   { $vars_forma_consulta[bib_id]=$bib_id;}
 $resultados = '';
 if (isset($vars_forma_consulta[sent])) {$resultados='<p id="resultadosObtenidos">Resultados obtenidos:'.$vars_forma_consulta[hits].'</p>';}

 $form['intro'] = array(
  '#markup'=>'
  <script>
    jQuery(document).ready(function(){
      jQuery("#edit-submit").click(function(){
        jQuery("#buscandoLibros").fadeIn();
        jQuery("body").scrollTop(0);
      });
    });
  </script>
  <div id="buscandoLibros"><h1><img src="http://conabip.gob.ar/sites/all/themes/cnbp/css/images/book.gif"/> Buscando en el Catálogo Colectivo</h1></div>
  <p id="catalogoIntro">En el marco del Plan de Inclusión Digital, se inició la informatización de la gestión bibliotecológica de las bibliotecas populares de Argentina. Las acciones incluyen la automatización de los catálogos bibliográficos, abriendo las puertas a un nuevo concepto de cooperación bibliotecaria a través de la creación del catálogo colectivo. <br><br>Aquí se puede consultar el acervo bibliográfico de las bibliotecas que están participando de este proceso.</p><br><hr>',
  );
 $form['query'] = array(
  '#type' => 'textfield',
  '#title' => t('B&uacute;squeda simple'),
  '#size' => 30,
  '#default_value' => $vars_forma_consulta[query],
  '#maxlength' => 64,
    //'#description' => t('Expresion a buscar'),
  );



 $form['avanzada'] = array(
  '#type' => 'fieldset',
  '#title' => t('Filtros de búsqueda'),
    '#collapsible' => TRUE, // Added
    '#collapsed' => TRUE,  // Added
    );

 $form['debug'] = array('#type' => 'value', '#value' => $debug);
 $form['qstart'] = array('#type' => 'value', '#value' => $vars_forma_consulta[start]);
//$form['campo'] = array(
//    '#type' => 'select',
//    '#title' => t('Buscar en'),
//    '#default_value' => $vars_forma_consulta[campo],
//    '#options' => array("1" => "Todos","2" => "Autor", "3" => "Titulo"),
//  );
 $form['hr1'] = array('#type' => 'item', 
  '#value' => '<hr>',
  );

  /* $form['avanzada']['buscar_en'] = array(
       '#type' => 'select',
       '#title' => t('Buscar en'),
       '#options' => array(
          0 => t('Bibliotecas y otras instituciones'),
          1 => t('Solo bibliotecas'),
          2 => t('Solo otras instituciones'),

       ),
       '#default_value' => 0,
       '#description' => t('Por defecto la búsqueda se realiza en todas las bibliotecas populares adheridas al sistema DigiBEPE y en instituciones participantes.'),
   );
*/
   $form['avanzada']['bp_nombre'] = array(
    '#type' => 'textfield',
    '#title' => t('Nombre de la biblioteca '),
    '#size' => 60,
    '#default_value' => $vars_forma_consulta[bp_nombre],
    '#maxlength' => 64,
    //'#description' => t('Expresion a buscar'),
    );





   $places=db_query('(SELECT DISTINCT province FROM location WHERE province=:capital ORDER BY province ASC)
    UNION
    (SELECT DISTINCT province FROM location WHERE province!=:empty  AND province!=:capital ORDER BY province ASC)', array(':capital'=>'Ciudad Autónoma de Buenos Aires',':empty'=>""));

   foreach ($places as $provincia) {

    $subItem=db_query('SELECT DISTINCT city FROM location WHERE province=:province ORDER BY city ASC',array(':province'=>$provincia->province));
    if(@count($subItem)>0){
      $ciudades=array();
      foreach ($subItem as $ciudad) {
        $ciudades[$ciudad->city]=array('label'=>$ciudad->city);
      }
    }
    $data[$provincia->province]=array(
      'label'=>$provincia->province,
      'children' =>$ciudades,
      );
  }


  $form['avanzada']['localidad']= array(
    '#type' => 'hierarchical_select',
    '#title' => t('Lugar'),
    '#size' => 1,
    '#config' => array(
      'module' => 'hs_smallhierarchy',
      'params' => array(
        'hierarchy' => $data,
        'id' => 'something-unique',
        'separator' => '|',
        ),
      'save_lineage'    => 0,
      'enforce_deepest' => 0,
      'entity_count'    => 0,
      'resizable'       => 1,
      'level_labels' => array(
        'status' => 0,
        'labels' => array(
          0 => t('Provincia o Región'),
          1 => t('Ciudad o Barrio'),

          ),
        ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'),
        'limit'    => 0,
        'reset_hs' => 1,
        ),
      'editability' => array(
        'status'           => 0,
        'item_types'       => array(),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
          ),
        'allow_new_levels' => 0,
        'max_levels'       => 3,
        ),
      // These settings cannot be configured through the UI: they can only be
      // overridden through code.
      'animation_delay'    => 400,
      'exclusive_lineages' => array(),
      'render_flat_select' => 0,
      ),
    '#description' => 'Seleccione el lugar para acotar la búsqueda',
    '#default_value' => $vars_forma_consulta[localidad],
    );


  $form['avanzada']['titulo'] = array(
    '#type' => 'textfield',
    '#title' => t('T&iacute;tulo'),
    '#size' => 30,
    '#default_value' => $vars_forma_consulta[titulo],
    '#maxlength' => 64,
    );
  $form['avanzada']['autor'] = array(
    '#type' => 'textfield',
    '#title' => t('Autor'),
    '#size' => 30,
    '#default_value' => $vars_forma_consulta[autor],
    '#maxlength' => 64,
    );
  $form['avanzada']['tema'] = array(
    '#type' => 'textfield',
    '#title' => t('Tema'),
    '#size' => 30,
    '#default_value' => $vars_forma_consulta[tema],
    '#maxlength' => 64,
    );
  $form['avanzada']['isbn'] = array(
    '#type' => 'textfield',
    '#title' => t('ISBN/ISSN'),
    '#size' => 30,
    '#default_value' => $vars_forma_consulta[isbn],
    '#maxlength' => 64,
    );
  $form['submit'] = array('#type' => 'submit', 
    '#value' => t('Consulta'),
    );


  $form['hits'] = array('#type' => 'item', 
    '#value' => $resultados, 
    );

  $form['Mensaje_error'] = array('#type' => 'item', 
    '#value' => "<span class='consulta_cc_error'> $vars_forma_consulta[mensaje_de_error] </span>",
    );
  if($vars_forma_consulta['hits']>0 && (! $vars_forma_consulta[mensaje_de_error])){
    $form['results'] = array
    (
      '#markup' => $resultados.$vars_forma_consulta['html']
      );


    $form['submit_prev'] = array('#type' => 'submit', 
      '#value' => t('Anteriores'),
      '#disabled' => $vars_forma_consulta[start] <2,
      );

    $form['submit_next'] = array('#type' => 'submit', 
     '#value' => t('Siguientes'),
     '#disabled' => (($vars_forma_consulta[hits] - $vars_forma_consulta[start]) < 100),
     );


  }

  unset($_SESSION['vars_forma_consulta']);
  return $form; 
}


/*
PROCESA LA CONSULTA
*/
function forma_consulta_submit($form, &$form_state) {   

//global $vars_forma_consulta; 
  $vars_forma_consulta=array();

  $vars_forma_consulta[sent]=1;
//if ($vars_forma_consulta[start] < 1) {print "init start"; $vars_forma_consulta[start]=1;}
  $vars_forma_consulta[start]=$form_state['values']['qstart'];
//print_r($form_state['values']['op']);
  if ($form_state['values']['op'] == 'Enviar consulta') $vars_forma_consulta[start] = 1;
  if ($form_state['values']['op'] == 'Siguientes') $vars_forma_consulta[start] += 100;
  if ($form_state['values']['op'] == 'Anteriores') 
    {$vars_forma_consulta[start] -= 100;
     if ($vars_forma_consulta[start] <1) $vars_forma_consulta[start]=1;
   }
//print " start:".$vars_forma_consulta[start];
   $host[] ="cc.bepe.ar:7090/biblios";
   $query = $form_state['values']['query'];
   $campo = $form_state['values']['campo'];
   $debug = $form_state['values']['debug'];
   $localidad = $form_state['values']['localidad']  ; 
   $bp_nombre = $form_state['values']['bp_nombre']  ;
   $autor = $form_state['values']['autor']  ;
   $tema = $form_state['values']['tema']  ;
   $isbn = $form_state['values']['isbn']  ;
   $titulo = $form_state['values']['titulo']  ;
//$bib_id = $form_state['values']['bib_id']  ; //Diego Here
   $num_hosts = count($host);



   //Ambito de la busqueda
   $ambito =  $form_state['values']['buscar_en'];



   $respuesta = ccbp_buscar($campo, $query, $localidad, $num_hosts, $host, $vars_forma_consulta[start], $autor, $titulo, $tema, $bib_id, $bp_nombre, $isbn, $debug, $ambito);


   $rows = $respuesta[0];
   $rows=array_slice($rows, 1);
   $html='<div class="row">';
   foreach($rows as $itemHtml){
    $html.='<div class="column col-md-12 libroCatalogo">'.$itemHtml[0].'<hr></div>';
  }
  $html.='</div>';

  $last_hits = $respuesta[1];
  $vars_forma_consulta[mensaje_de_error] =  $respuesta[2];

  $vars_forma_consulta[query]= $query;
  $vars_forma_consulta[loc]=$localidad;
  $vars_forma_consulta[bp_nombre]=$bp_nombre;
  $vars_forma_consulta[campo]=$campo;
  $vars_forma_consulta[autor]=$autor;
  $vars_forma_consulta[tema]=$tema;
  $vars_forma_consulta[titulo]=$titulo;
  $vars_forma_consulta[isbn]=$isbn;
  $vars_forma_consulta[hits]=$last_hits;
  $vars_forma_consulta[html]=$html;


  $_SESSION['vars_forma_consulta']=$vars_forma_consulta;

}





/*
FUNCIONES HELPERS
*/

// genera la lista de bp que cumple con una o dos condiciones
// $id es el nid de una localidad (taxonomia)
// $bp_nombre es el nombre de una biblioteca (str)

function condicion_bib($id, $bp_nombre,$ambito = null)
{
/************************************************************************************/
//Filtro por Nombre
 if(($id[0]=="" || $id=="") && $bp_nombre!=""){
    $typeSearch='BUSQUEDA SOLO POR NOMBRE BP';
  
    if($ambito==2){
      $minReg=10000;
    }
    else {
      $minReg=0;
    }
    $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.entity_type=:user AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND reg.field_n_mero_de_registro_value > :minReg AND name.field_nombre_de_la_biblioteca_value LIKE :like',array(':user'=>'user',':rol'=>4, ':minReg'=>$minReg,':like'=>'%'.$bp_nombre.'%'));
}
/************************************************************************************/
//Filtro por Localidad
if($id!="" && $bp_nombre==""){

 $loc=explode("|", $id);

 if(@count($loc)==2){
  $typeSearch='BUSQUEDA SOLO POR LOCALIDAD y PROVINCIA';
  $provincia=$loc[0];
  $ciudad=$loc[1];
  $minReg=0;
  
  if($ciudad!=""){
    $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.entity_type=:user AND reg.field_n_mero_de_registro_value > :minReg AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND d.province =:province AND d.city=:city',array(':user'=>'user', ':minReg'=>$minReg,':rol'=>4, ':province'=>$provincia,':city'=>$ciudad));
  }
  else{
    $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.entity_type=:user AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND d.province =:province',array(':user'=>'user',':rol'=>4, ':province'=>$provincia));
  }

}
else{
  $provincia=$loc[0];
  $typeSearch='BUSQUEDA SOLO POR PROVINCIA';
  $minReg=0;

  $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.field_n_mero_de_registro_value>:minReg AND reg.entity_type=:user AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND d.province =:province',array(':minReg'=>$minReg ,':user'=>'user',':rol'=>4, ':province'=>$provincia));
}




}
 


  //Filtro por Nombre y Localidad
if($id!="" && $bp_nombre!=""){
 
 $loc=explode("|", $id);

 if(@count($loc)==2){
  $typeSearch='BUSQUEDA  POR LOCALIDAD, PROVINCIA y NOMBRE BP';
  $provincia=$loc[0];
  $ciudad=$loc[1];
  $minReg = 0;
  if($ciudad!=""){
    $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.entity_type=:user AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND reg.field_n_mero_de_registro_value >:minReg AND d.province =:province AND d.city=:city AND name.field_nombre_de_la_biblioteca_value LIKE :like',array(':user'=>'user',':rol'=>4, ':minReg'=>$minReg,':province'=>$provincia,':city'=>$ciudad,':like'=>'%'.$bp_nombre.'%'));
  }
  else{

    $typeSearch='BUSQUEDA  POR PROVINCIA y NOMBRE BP (ALT)';
    $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.entity_type=:user AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND reg.field_n_mero_de_registro_value>:minReg AND d.province =:province AND name.field_nombre_de_la_biblioteca_value LIKE :like',array(':user'=>'user',':rol'=>4, ':minReg'=>$minReg, ':province'=>$provincia,':like'=>'%'.$bp_nombre.'%'));
  }

}
else{


  try {
      $typeSearch='BUSQUEDA  POR PROVINCIA y NOMBRE BP';
      $provincia=$loc[0];
      $minReg=0;

      $result=db_query('SELECT name.entity_id AS uid, name.field_nombre_de_la_biblioteca_value AS name, reg.field_n_mero_de_registro_value AS reg, d.city, d.province FROM field_data_field_nombre_de_la_biblioteca name, field_data_field_n_mero_de_registro reg, users_roles rol, location d, (SELECT l.lid, l.uid FROM location_instance l) AS loc WHERE loc.lid = d.lid AND reg.entity_id=loc.uid AND reg.entity_type=:user AND reg.entity_id=name.entity_id AND rol.uid=reg.entity_id AND rol.rid=:rol AND reg.field_n_mero_de_registro_value>:minReg  AND d.province =:province AND name.field_nombre_de_la_biblioteca_value LIKE :compara',array(':user'=>'user',':rol'=>4, ':minReg'=>$minReg,':province'=>$provincia,':compara'=>'%'.$bp_nombre.'%'));

    } catch (Exception $e) {
        print 'Caught exception: '.$e->getMessage();
        die();
    }


}




}



$condicion_id = '';
$condicion_nombre = '';

$bibs = '';
$or1='';
$con_digibepe=0;

//generacion de la condicion en base a los titulos (en realidad # de bib)

foreach($result as $record){

  if ($bibs) {$or1 = $or1." @or ";}
  $bibs=$bibs.' @attr 1=56 "'.$record->reg.'" ';
  $con_digibepe += 1;
  

}




$error = '';
if ($debug) print_r($bibs);
if ((! $bibs) or (! $con_digibepe))
  {  $razon1 = '';
$razon2 = '';
$conectivo = '';
$digitalizadas = '';
           //if ((! $con_digibepe) && ($bibs))
           //   $digitalizadas = 'utilizando DIGIBeP&eacute; 2010 ';
if ($id) $razon1 = "en la ubicaci&oacute;n especificada";
if ($bp_nombre) $razon2 = "con el nombre especificado";
if ( ($razon1) && ($razon2) ) $conectivo = ' o ';
$no_hay = "No se encontraron bibliotecas populares ";
$error = $no_hay.$digitalizadas.$razon1.$conectivo.$razon2;
if ((! $con_digibepe) && ($bibs))
{
 if ($id) $error = "La(s) Biblioteca(s) Popular(es) de la ubicaci&oacute;n especificada no se encuentra(n) registrada(s) en el Cat&aacute;logo Colectivo de la Conabip";
 if ($bp_nombre) $error = "La(s) Biblioteca(s) Popular(es) con el nombre especificado no se encuentra(n) registrada(s) en el Cat&aacute;logo Colectivo de la Conabip";

}
}
$resultado = array($or1.$bibs, $error);
return $resultado;


}

//Limpiar cadena
function clean_str($a)
{ 
  $b = $a; 
  $b=str_replace(",","",$b);
  $b=str_replace(" ","",$b);
  $b=str_replace(".","",$b);
  $b=str_replace(";","",$b);
  $b=str_replace("/","",$b);
  $b = strtolower($b);
  return $b;

}

//Obtener Datos BP y link
function nombrebp($reg)
{
  $result=db_query('SELECT r.entity_id as uid , r.field_n_mero_de_registro_value  as reg , bp.field_nombre_de_la_biblioteca_value as name, ld.city, ld.province FROM field_data_field_n_mero_de_registro r, users_roles u , field_data_field_nombre_de_la_biblioteca bp, location_instance l , location ld WHERE r.bundle=:bundle AND r.field_n_mero_de_registro_value=:reg AND u.uid=r.entity_id AND u.rid=4 AND bp.entity_id=u.uid AND bp.entity_id=r.entity_id AND bp.bundle=:bundle AND l.uid=u.uid AND ld.lid=l.lid',array(':reg'=>$reg,':bundle'=>'user'));

  foreach($result as $record){
    print_r($record);

    $link=l($record->name.'(Reg: '.$reg.'),'.$record->city.','.$record->province,'user/'.$record->uid);

  }

  return $link; 

}

//Obtener datos ZEBRA by tag
function getvaluebytag($index, $vals, $mytag, $mycode)
{
  $indexfield = $index['DATAFIELD'];
  $tipo =  gettype($indexfield);
  //print_r($index);
  if ($tipo != 'array')  //situacion anomala
      {  //print_r($index);
            //print_r($vals);

       return "";
     }

     foreach ($indexfield as $k=>$v)
       {if ($vals[$v]['type'] == 'open')
     { if ($vals[$v]['attributes']['TAG'] == $mytag)
     { 

       $v2 = $v + 1;

       while (($vals[$v2][type] != 'close') && ($vals[$v2]['attributes']['CODE'] != $mycode))
       { 
        $v2++; 

      }
      if ($vals[$v2]['attributes']['CODE'] == $mycode) 
      {
       $valor = $vals[$v2]['value'];
       return $valor;
     }
   }
 }
}
return '';
}

//Versión alternativa para obtener values del zebra
function getvaluebytag2($index, $vals, $mytag, $tagtype)
{
  $indexfield = $index[$tagtype];
  //print_r($indexfield);
  $tipo =  gettype($indexfield);
  //print_r($index);
  if ($tipo != 'array')  //situacion anomala
      {  //print_r($index); 
            //print_r($vals);

       return "no";
     }
     foreach ($indexfield as $k=>$v)
     {  //print "checando ".$v." ".$vals[$v]['attributes']['TAG']." contra ".$mytag."<br>";
   if ($vals[$v]['attributes']['TAG'] == $mytag)
   {
    $valor = $vals[$v]['value'];
    return $valor;

  }
}
return 'no';
}


// Convertir el XML a array
function xmltoarray($rec)
{
  $parser = xml_parser_create();
  xml_parse_into_struct($parser, $rec, $vals, $index);
  xml_parser_free($parser);
  return array('index' => $index, 'vals' => $vals);
}


//Sumar commillas
function comillas($st)
{
  return '"'.$st.'"';
}



//Obtener resultados!
function ccbp_buscar($campo, $query, $localidad, $num_hosts, $host, $start, $qautor, $qtitulo, $qtema, $bib_id, $bp_nombre, $isbn, $debug, $ambito)
{



  $s = '';
  $prefijo='';

  print '<script type="text/javascript" src="http://books.google.com/books/previewlib.js"></script>';
  $query = trim($query);
  $qtitulo = trim($qtitulo);
  $bp_nombre = trim($bp_nombre);
  switch($campo) {
    case "2": $prefijo=" @attr 1=1003 @attr 4=1 ";
    break;
    case "3": $prefijo=" @attr 1=4 ";
    break;
  }
  if ($query) {$finalquery = $prefijo.comillas($query);}

  if ($qtitulo){$myquery = "  @attr 1=4  ".comillas($qtitulo);
  if ($finalquery) $finalquery = "@and ".$finalquery." ".$myquery;
  else {
    $finalquery = $myquery;
  }
}

if ($qautor) {

  $myquery = "  @attr 1=1003 @attr 4=1 ".comillas($qautor);


  if ($finalquery) {$finalquery = "@and ".$finalquery." ".$myquery;}
  else{$finalquery = $myquery;}       

}
if ($qtema){
  $myquery = "  @attr 1=21  ".comillas($qtema);
  if ($finalquery) {$finalquery = "@and ".$finalquery." ".$myquery;}
  else{$finalquery = $myquery;}                         
}

if ($isbn){
  $myquery = "  @or @attr 1=7  ".comillas($isbn)." @attr 1=8  ".comillas($isbn);
  if ($finalquery){ $finalquery = "@and ".$finalquery." ".$myquery;}
  else{
   $finalquery = $myquery;}
 }
 if ($bib_id) {
  $myquery = "  @attr 1=56 ".comillas($bib_id);
  if ($finalquery) {$finalquery = "@and ".$finalquery." ".$myquery;}
  else {$finalquery = $myquery;}                         
}
$error_condicion_loc = '';
$bp_nombre = str_replace( "'", "", $bp_nombre);


if (($localidad[0]) || ($bp_nombre)) {

 $resultado_condicion_loc=condicion_bib($localidad[0], $bp_nombre,$ambito);
        // print_r($resultado_condicion_loc); die();
 $condicion_loc = $resultado_condicion_loc[0];
 $error_condicion_loc = $resultado_condicion_loc[1];
 $finalquery = "@and ".$condicion_loc." ".$finalquery;
}


if ($debug) {echo 'You searched for ' . htmlspecialchars($finalquery) . '<br />';}


for ($i = 0; $i < $num_hosts; $i++) {

 try {
  $id[] = yaz_connect($host[$i]);
} catch (Exception $e){
  print "---------Inicio ERROR yaz Connect----------<br>";
  print_r($id);
  print_r($e);
  print "<br>---------FIN ERROR yaz Connect----------<br>";

}



yaz_syntax($id[$i], "usmarc");
        //yaz_range($id[$i], 1, 10);
yaz_sort($id[$i], "1=1003 ia 1=4 ia");
yaz_search($id[$i], "rpn", $finalquery);

}

yaz_wait();



for ($i = 0; $i < $num_hosts; $i++) {

        //$s .= "debug ".$debug;
  $error = yaz_error($id[$i]);
  $hits=0;
  if (!empty($error)) {
    echo "Error: $error";
  } else {
    $hits = yaz_hits($id[$i]);
            //echo "Result Count $hits";
  }
  $table_results=array();
  $table_results['data']=array();
  $p =$start;
  $max_records = 100;
  $end = $p + $max_records;
  if ($hits)
  {
    $rec = yaz_record($id[$i], $p, "xml");

    $res = xmltoarray($rec);
          //print_r($res);

    $index = $res['index'];
    $vals  = $res['vals'];
    $nrec = getvaluebytag($index, $vals, '999','c');
    if ($debug)
    {
     print "<code>".$rec."</code>";
     print '<pre>';print_r($index);print '</pre>';
     print '<pre>';print_r($vals);print '</pre>';
   }


   $autor_actual=getvaluebytag($index, $vals, '100','a');
   $autor = $autor_actual;
   $autor_c = clean_str($autor_actual);
   $autor_actual_c = clean_str($autor_actual);



   $titulo_actual=getvaluebytag($index, $vals, '245','a');
   $titulo = $titulo_actual;
   $titulo_actual_c = clean_str($titulo_actual);
   $titulo_c = $titulo_actual_c;

   $nrec_actual = getvaluebytag($index, $vals, '999','c');

        //$bib = getvaluebytag2($index, $vals, '003','CONTROLFIELD');
   $bib = getvaluebytag($index, $vals, '040','a');
   if (! $bib) $bib = getvaluebytag($index, $vals, '952','b');
   $nombrebp = nombrebp($bib);
   $nbib=0;







   $bibs = array();
   while (($p <= $end) && ($rec) ) {
    while (($autor_c == $autor_actual_c) && ($titulo_c == $titulo_actual_c) && ($rec) && ($p <= $end))
    {
      if (isset($bibs[$bib]))
        {$bibs[$bib]++;}
      else
        {$bibs[$bib]=1;}
                //$bibs[$nbib]=$bib;
                //$nbib++; 
      $p++;
      $rec = yaz_record($id[$i], $p, "xml");
              //print_r($rec);
              //print "------";
      $res = xmltoarray($rec);
      $index = $res['index'];
      $vals  = $res['vals'];
      if ($rec)
      {
              //print_r($rec);
        $autor= getvaluebytag($index, $vals, '100','a');
        $autor_c = clean_str($autor);
        $titulo= getvaluebytag($index, $vals, '245','a');
        $titulo_c = clean_str($titulo);
        $ed = getvaluebytag($index, $vals, '250','a');
        $bib = getvaluebytag($index, $vals, '040','a');
        if (! $bib) $bib = getvaluebytag($index, $vals, '952','b');
                //$bib = getvaluebytag2($index, $vals, '003','CONTROLFIELD');
        $nrec = getvaluebytag($index, $vals, '999','c');
        $nombrebp = nombrebp($bib  );
        $editorial = getvaluebytag($index, $vals, '260', 'b');
        $anio = getvaluebytag($index, $vals, '260', 'c');
        $lugar = getvaluebytag($index, $vals, '260', 'a');
        $isbn = getvaluebytag($index, $vals, '20', 'a');

        $soporte = getvaluebytag($index, $vals, '942', 'c');
        if (! $soporte) $soporte = getvaluebytag($index, $vals, '952', 'y');

      }

               } // while autor.....
               $sbibs = array();
               $id_bibs = '';
               foreach ($bibs as $k => $v)
               {
                 $sbibs[]= '<span class="bp">'.nombrebp($k)." ($v) </span> "; 
               }
               $sbibs= implode('.<br>',$sbibs).'.';
                //$liga_titulo_actual = "<a href=/cons_ccbp_item?bib_id=".$nrec_actual."&bibs=$id_bibs>".$titulo_actual."</a>";  
                //$table_results[]=array($titulo_actual, $autor_actual, $sbibs);
               

               if (strlen($isbn)>3) {

                $infoJson = file_get_contents('https://www.googleapis.com/books/v1/volumes?q=isbn:'.str_replace('-','',$isbn));
                $infoBook=json_decode($infoJson);

                //$imgUrl='<img class="bookImage" alt="'.$titulo.'" src="http://www.librarything.com/devkey/1caac53c0e75f0d77a8c62ff2bf46c8a/large/isbn/'.str_replace('-','',$isbn).'"/>';

                $gbook=$infoBook->items[0];
                if($gbook->volumeInfo->imageLinks->thumbnail !=""){$gbookImage='<img class="bookImage" alt="'.$titulo.'" src="'.$gbook->volumeInfo->imageLinks->thumbnail.'"/>';}  
                //else {$gbookImage=$imgUrl;}
                else {$gbookImage="<div class=\"bookCover\"><br/><br/><p>".$titulo."</p></div>";}
                
              }
              else {$gbookImage="<div class=\"bookCover\"><br/><br/><p>".$titulo."</p></div>";}


              $renglon = "<div class=\"row\">
              <div class=\"column col-md-3\">$gbookImage</div>
              <div class=\"column col-md-9\">
               <h3>$titulo</h3>";
               if ($autor_actual)
               {
                $renglon .= "<h4> por ";
                $renglon .= "$autor_actual</h4>";
              }
              if (strlen($isbn)>3) {






                $renglon.='<div class="item"><label>ISBN:</label> '.str_replace('-','',$isbn).'</div>';
                //MODIF

                /*$renglon.= '
                <div class="gpreview">'.$gbookImage.'
                <script type="text/javascript">
                GBS_setLanguage(\'es\');
                GBS_insertPreviewButtonPopup(\'ISBN:'.str_replace('-','',$isbn).'\');
                </script><br/></div>
                ';*/

              }    
            //if ($isbn) $renglon .= '<div class="item"><label>ISBN</label>'.$isbn.'</div>';
              if ($issn) $renglon .= '<div class="item"><label>ISSN:&nbsp;</label>'.$issn.'</div>';
              if ($soporte) $renglon .= '<div class="item"><label>Soporte:&nbsp;</label>'.$soporte.'</div>';
              $renglon .= '<div class="item"><label>Publicacion:&nbsp;</label>'.$lugar.' '.$editorial.' '.$anio.'</div>';
              $renglon .= '<div class="item"><label>Disponibilidad:&nbsp;</label>'.$sbibs.'</div>';
              $renglon .='</div></div>';
            //$table_results[]=array($titulo_actual, $autor_actual, $sbibs);
              $table_results[]=array($renglon);
              $titulo_actual = $titulo;
              $titulo_actual_c = clean_str($titulo);
            //$titulo_c = $titulo_actual;
              $autor_actual =  $autor;
              $autor_actual_c =  clean_str($autor);
              $nrec_actual = getvaluebytag($index, $vals, '999','c');
            //$autor_c =  clean_str($autor);

              $bibs = array();
              $nbib = 0;
            //$p++;
            } //while (($p <= $end) && $rec)
          }
} //if $hits
//return $s;


return array($table_results, $hits, $error_condicion_loc);

}




function cons_ccbp_all() {
  return drupal_get_form('forma_consulta');

}

function cons_ccbp_item() {
  return drupal_get_form('forma_item');

}

function forma_consulta_validate($form, &$form_state) {
  if ( ($form_state['values']['query'] == '') && ($form_state['values']['autor'] == '')  && ($form_state['values']['titulo'] == '')  && ($form_state['values']['tema'] == '') && ($form_state['values']['bib_id'] == '') && ($form_state['values']['isbn'] == '')) {
    form_set_error('', t('La consulta debe tener al menos una busqueda simple o autor o t&iacute;tulo o ISBN.'));
  }
}




function selectlocalidad()
{ $r = taxonomy_get_tree(6);
  $f = '';
  $f .=  '<SELECT NAME="localidad">';
  $f .= '<OPTION VALUE="">Todas';

  foreach ($r as $k=>$v)
    {  $pref='';
  if ($v->depth) $pref="--";
  $f .= '<OPTION VALUE="'.$v->tid.'">'.$pref.$v->name;


}
$f .= '</select>';
return $f;


}

