/*
 * Dynamic Table of Contents script
 * by Matt Whitlock <http://www.whitsoftdev.com/>
 */

function createLink(href, innerHTML) {
   var a = document.createElement("a");
   a.setAttribute("href", href);
   a.innerHTML = innerHTML;
   return a;
}

   var i2 = 0, i3 = 0, i4 = 0;max_div_depth = 5;
function generateTOC(toc) {
   toc = toc.appendChild(document.createElement("ul"));
   //var base_node = document.body;
   var base_node = document.getElementById("content");
   search_h_tags(base_node);
}


function search_h_tags(cur_node)
{
   for (var i = 0; i < cur_node.childNodes.length; ++i) 
   {
      var node = cur_node.childNodes[i];
      var tagName = node.nodeName.toLowerCase();
      
      if (tagName == "div") {
         max_div_depth--;
         if (max_div_depth > 0)
            search_h_tags(node);
         max_div_depth++;   

      } else if (tagName == "h4") 
      {
         ++i4;
         if (i4 == 1) toc.lastChild.lastChild.lastChild.appendChild(document.createElement("ul"));
         var section = i2 + "." + i3 + "." + i4;
         node.insertBefore(document.createTextNode(section + ". "), node.firstChild);
         if (node.id == "")
         {
            node.id = "section" + section;
            toc.lastChild.lastChild.lastChild.lastChild.appendChild(document.createElement("li")).appendChild(createLink("#section" + section, node.innerHTML));
         } else {
            toc.lastChild.lastChild.lastChild.lastChild.appendChild(document.createElement("li")).appendChild(createLink("#" + node.id, node.innerHTML));
         }
      }
      else if (tagName == "h3") 
      {
         ++i3, i4 = 0;
         if (i3 == 1) toc.lastChild.appendChild(document.createElement("ul"));
         var section = i2 + "." + i3;
         node.insertBefore(document.createTextNode(section + ". "), node.firstChild);
         if (node.id == "")
         {
            node.id = "section" + section;
            toc.lastChild.lastChild.appendChild(document.createElement("li")).appendChild(createLink("#section" + section, node.innerHTML));
         } else {
            toc.lastChild.lastChild.appendChild(document.createElement("li")).appendChild(createLink("#" + node.id, node.innerHTML));
         }
      }
      else if (tagName == "h2")
      {
         ++i2, i3 = 0, i4 = 0;
         var section = i2;
         node.insertBefore(document.createTextNode(section + ". "), node.firstChild);
         if (node.id == "") 
         {
            node.id = "section" + section;
            toc.appendChild(h2item = document.createElement("li")).appendChild(createLink("#section" + section, node.innerHTML));
         } else {
            toc.appendChild(h2item = document.createElement("li")).appendChild(createLink("#" + node.id, node.innerHTML));
         }
      }
   }
}
