<?php if($type=='bpWall'):?>
	<p>&nbsp;&nbsp;&nbsp;<a href="http://www.conabip.gob.ar/bipop/" class="btn btn-default"><span class="glyphicon glyphicon-chevron-left"></span> Volver al muro Bipop</a></p>
<?php endif;?>
<div id="headerBipop" class="<?php echo $type;?> row">
	 <div class="imageHeaderBipop col col-md-4">
	 	<?php echo $image;?>
	 </div>
	 <div class="infoBipop col col-md-8">
	 	<h1><?php echo $bp;?></h1>
	 	<div class="bpData">
	 		<?php echo $address;?>
	 	</div>
	 	<div class="bpServices">
	 		<?php echo $social;?><br>
	 		<?php echo $services;?>
	 	</div>
	 </div>

<div class="clear"></div>
</div>





<style type="text/css">
	
	#headerBipop {
    background-color: #489cd6 ;
    height: auto !important;
    padding-bottom: 30px !important
  
	}
	#headerBipop .imageHeaderBipop {
		padding:0;
	}
	#headerBipop .imageHeaderBipop img {
		width: 100%;
		height: 100%;
	}
	#headerBipop .bipopServices {
		margin-top: 20px;
	}

	#headerBipop .bipopServices h3 {
    color: #fff;
   
	}
	#headerBipop  .glyphicon.glyphicon-map-marker {
    font-size: 40px;
    float: left;
    color: #fff;
    margin-right: 10px;
   
}
</style>

