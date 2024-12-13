<?php
$servername = 'localhost';
$username = ''; //username for dBase
$password = ''; //Password for dBase
$dbname = ''; //dBase name



$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

// Perform the query
$sql = "SELECT node_name,  time_received, voltage FROM sensor_data";
$result = $conn->query($sql);

// Display the Data
$i=0;
if ($result->num_rows > 0) {
  // output data of each row
  echo "<style>
  table , th, td {
    border:1px solid black
    }
    tr:nth-child(even) {
    background-color: #FF8DC6;
}
    tr:nth-child(odd) {
    background-color: #FCA9D3;
}

      </style>";
  echo "<table style='margin-left: auto; margin-right: auto;'>
<th>time recieved</th>
<th>voltage</th>";
$avtemp = 0;
$avhum = 0;
$num = 0;
  while($row = $result->fetch_assoc()) {
            echo "<tr><td>";

            $TIME[]  = $row['time_received']  ;
            echo "$TIME[$i]   </td>";
            echo "<td>";
            $nodetemp[]  = $row['voltage']  ;
            echo "$nodetemp[$i]   </td>";
            if($nodename[$i] == 'node-1'){
                $avtemp = $avtemp + $nodetemp[$i];
                $avhum = $avhum + $nodehum[$i];
                $num = $num + 1;
            }
            $i=$i+1;
            
  } 
  echo "</table>";

  
}




try {
    // Create a PDO database connection
    $db = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);

    // SQL query to retrieve data
    $query = "SELECT time_received, voltage FROM sensor_data";

    // Prepare and execute the query
    $stmt = $db->prepare($query);
    $stmt->execute();

    // Fetch data into an array
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Close the database connection
    $db = null;
} catch (PDOException $e) {
    die("Error: " . $e->getMessage());
}

// Prepare data for Google Charts
$dataArray = [['Time Received', 'Voltage']];
foreach ($data as $row) {
    $dataArray[] = [$row['time_received'], (float) $row['voltage']];

}
$colors[] = 'pink';
$colorsJson = json_encode($colors);
$dataJson = json_encode($dataArray);
//echo $dataJson;
?>



<!DOCTYPE html>
<html>
<head>
    <title>Voltage Chart</title>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
        google.charts.load('current', {'packages':['corechart']});
        google.charts.setOnLoadCallback(drawChart);

        function drawChart() {
            var data = google.visualization.arrayToDataTable(<?php echo $dataJson; ?>);

            var options = {
                title: 'Voltage vs Time',
                vAxis: { title: 'Voltage' },
                hAxis: { title: 'Time', slantedText: true },
                series: {0: { pointSize: 6 }},
                colors: <?php echo $colorsJson; ?>,
                curveType: 'function',
                legend: { position: 'none' }
            };

            var chart = new google.visualization.ColumnChart(document.getElementById('chart_div'));

            chart.draw(data, options);
        }
    </script>
</head>
<body>
    <div id="chart_div" style="width: 800px; height: 400px;"></div>
</body>
</html>
