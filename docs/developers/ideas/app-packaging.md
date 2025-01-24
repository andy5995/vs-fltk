At the moment the way `app` is designed is not really great in my opinion.  
It lacks the ability to properly describe information about it and its distribution.  
Also, it forces the distribution of an application in multiple files, which might not always be desirable.  
Even less so if the plan is to distribute them as single executables at some point.  
So...

```xml
<package framework="vs.fltk" name="..." origin="url" type="app" version=""> <!-- types can be app/lib/patch/data for now -->
    <!-- Strings for internationalization -->
    <i18n src="..." default="en-uk">
        <!--This part is very limited and only related to this specific file-->
        <entry tag="..." desc="...">
            <locale lang="en" country="uk">...</locale>
        </entry>
    </i18n>

    <metadata>
        <authors>
            <entry src="..." covers="" excludes=""/>
        </authors>
        <licences>
            <entry src="..." covers="" excludes=""/>
        </licences>
        <changelog>
            <entry src="..." version=""/>
        </changelog>
        <links>
            <entry ref="..." label="...">
        </links>
        <signature authority="..." value="..."/>
        <package-id>...</package-id>
        <icon></icon>
        <description></description>
        <repo ref="..."/>
    </metadata>

    <link-with>
        <entry src.object="" src.header=""/>
    </link-with>

    <policies>
    </policies>

    <deps>
        <dep name="" src="" version="" excludes="" signature=""/>  <!--body is foldback-->
        <patch for="" src="" signature=""/> <!--body is foldback-->
    </deps>

    <entry src="...">
       <!--Alternative if `src` fails or is not specified -->
    </entry>
</package>
```
