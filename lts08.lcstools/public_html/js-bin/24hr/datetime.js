now = new Date();

localtime = now.toString();
utctime = now.toGMTString();

document.write("<b>Local Time:</b> " + localtime + "<br>");
document.write("<b>UTC Time  :</b> " + utctime);

hours = now.getHours();
minutes = now.getMinutes();
seconds = now.getSeconds();

document.write("<h1>");
document.write(hours+":"+minutes+":"+seconds);
document.write("</h1>");
