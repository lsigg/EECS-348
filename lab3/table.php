<?php
$n = $_POST["num"];

echo "<table border='1'>";

// header row
echo "<tr><th></th>";
for ($i = 1; $i <= $n; $i++) {
  echo "<th>$i</th>";
}
echo "</tr>";

// table body
for ($i = 1; $i <= $n; $i++) {
  echo "<tr>";
  echo "<th>$i</th>";
  for ($j = 1; $j <= $n; $j++) {
    echo "<td>" . ($i * $j) . "</td>";
  }
  echo "</tr>";
}

echo "</table>";
?>