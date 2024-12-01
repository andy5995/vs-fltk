```xml
<app>
    <mixin name="..." args.../>
    <mixins src=""/>

    <item mixin="class1,class2,class3" overrides.../>
</app>
```

Mixins can be for:
- "*" Everyone
- "ns:*" Everyone in xml namespace ns
- "+tag1,+tag2,..." a list of tags
- "class1,class2" elements with that token in the `mixin` attribute