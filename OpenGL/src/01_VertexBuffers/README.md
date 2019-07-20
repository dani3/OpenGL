# Vertex buffers

Having an array of vertices defined:

```c++
float positions[6] = {
    -0.5f, -0.5f,
     0.0f,  0.5f,
     0.5f, -0.5f
};
```

We can __create__ a new buffer:

```c++
glGenBuffers(1, &buffer);
```

where:

* First argument specifies the __number__ of buffers.
* The second argument will be used to store the __identifier__ of that buffer.

then select it:

```c++
glBindBuffer(GL_ARRAY_BUFFER, buffer);
```

and populate it:

```c++
glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
```

Once this is done, we need to specifiy the __Vertex attributes__, in this case, we only use the position, so only one attribute:

```c++
// Enable first attribute
glEnableVertexAttribArray(0);

// Specify the attribute
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void *) 0);
```

Where:

* The first parameter is the __index__ of the attribute.
* The second parameter is the __size__ of the attribute (two component attribute).
* The third parameter sets the __type__.
* The fourth parameter indicates if the data has to be __normalized__ (floats are normalized).
* The fifth parameter indicates the size of the _stride_, ie. the offset to the next attribute.
* The sixth parameter indicates the offset to the first attribute.
