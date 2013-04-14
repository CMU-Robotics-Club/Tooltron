window.onload = function() {
  var rfid = document.getElementById("rfid");
  var button = document.getElementById("refresh");
  var errors = document.getElementById("errors");
  
  button.onclick = function() {
    var req = new XMLHttpRequest();
    req.open("GET", "/get-rfid", true);
    req.send();
    req.onreadystatechange = function() {
      if (req.readyState == 4) {
        if (req.status == 200) {
          rfid.value = req.responseText.replace(/\s/g, '');
          errors.innerText = '';
        } else {
          errors.innerText = 'Error '+req.status+'\n'+req.responseText;
        }
      }
    };
  };
  
  button.onclick();
};
