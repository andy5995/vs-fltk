#pragma once
/*
    Compile an xml file representing a component into a .so library which can just be loaded.
    Extra speed for a decrease in portability.
    However if the compiling target is C we can use TCC to make magic.
*/