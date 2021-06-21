<!DOCTYPE html>
<html>
	<head>
		<title>ChartJS - LineGraph</title>
		<meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
  <script>
     var time = new Date().getTime();
     $(document.body).bind("mousemove keypress", function(e) {
         time = new Date().getTime();
     });

     function refresh() {
         if(new Date().getTime() - time >= 5000) 
             window.location.reload(true);
         else 
             setTimeout(refresh, 5000);
     }

     setTimeout(refresh, 5000);
</script>
		<style>
			.chart-container {
				width: 95%;
				
				height: auto;
			}
		</style>
	</head>
	<body>
		<div class="chart-container">
			<div class="row">
				<div class="col-lg-6">
					<canvas id="mycanvas"></canvas>
				</div>
				<div class="col-lg-6">
					<canvas id="mycanvas1"></canvas>
				</div>
			</div>
			
			
		</div>
		
		<!-- javascript -->
		<script type="text/javascript" src="js/jquery.min.js"></script>
		<script type="text/javascript" src="js/Chart.min.js"></script>
		<script type="text/javascript" src="js/linegraph.js"></script>
	</body>
</html>