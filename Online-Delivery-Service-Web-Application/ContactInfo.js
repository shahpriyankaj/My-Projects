function loadDoc() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "contactinfo.xml", true);
    xhttp.send(null);
    xhttp.onreadystatechange = function ()
    {
        if (xhttp.readyState == 4 && xhttp.status == 200)
        {
                getContactDetails(xhttp);
        }
    };     
}
function getContactDetails(xml) {
    
    var i;
    var xmlDoc = xml.responseXML;
    root = xmlDoc.documentElement;
    var info = root.getElementsByTagName("info");
    
    var contact = "";
    contact += "<tr><td><h2>Contact Us</h2></td></tr>"
             + "<tr><td><strong>Texans serving Texans</strong></td></tr>"
             + "<tr><td><strong>24 Hours a Day!</strong></td></tr>"
             + "<tr><td><h2>Toll Free</h2></td></tr>"
             + "<tr><td><strong><u>" + info[0].getAttribute("toll-free-phone") + "</u></strong></td></tr>"
             + "<tr><td><h2>Houston</h2></td></tr>"
             + "<tr><td><strong><u>" + info[0].getAttribute("houston-phone") + "</u></strong></td></tr>"
            + "<tr><td><h2>E-mail</h2></td></tr>"
             + "<tr><td><strong><u>" + info[0].getAttribute("contact-email") + "</u></strong></td></tr>";
    document.getElementById("phoneDetails").innerHTML = contact;
    

    if (document.getElementById("contactDetails")) {
        var table = "";
        table += "<tr><td>" + info[0].getAttribute("city-one") + "</td><td>"
        + info[0].getAttribute("city-two") + "</td><td>"
        + info[0].getAttribute("city-three") + "</td></tr>" +
        "<tr><td><u>" + info[0].getAttribute("houston-phone") + "</u></td><td><u>"
        + info[0].getAttribute("austin-phone") + "</u></td><td><u>"
        + info[0].getAttribute("dallas-phone") + "</u></td></tr>" +
        "<tr><td>" + info[0].getAttribute("city-four") + "</td><td>"
        + info[0].getAttribute("city-five") + "</td><td>"
        + info[0].getAttribute("city-six") + "</td></tr>" +
        "<tr><td><u>" + info[0].getAttribute("elpaso-phone") + "</u></td><td><u>"
        + info[0].getAttribute("sanantonio-phone") + "</u></td><td><u>"
        + info[0].getAttribute("riogrande-phone") + "</u></td></tr>" +
        "<tr><td>" + info[0].getAttribute("city-seven") + "</td><td>"
        + info[0].getAttribute("city-eight") + "</td><td>"
        + info[0].getAttribute("city-nine") + "</td></tr>" +
        "<tr><td><u>" + info[0].getAttribute("corpus-phone") + "</u></td><td><u>"
        + info[0].getAttribute("easttx-phone") + "</u></td><td><u>"
        + info[0].getAttribute("westtx-phone") + "</u></td></tr>";
        document.getElementById("contactDetails").innerHTML = table;
    }
}
