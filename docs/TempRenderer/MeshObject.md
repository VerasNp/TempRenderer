## Malhas triangulares

```json
{
	// Nome do componente
	"name": "<string>",
	"type": "mesh",
	// Caminho até o arquivo ".obj" de malha
	"objFilePath": "<string>",
	"material": {
		// Componente de reflexão ambiente
		"ka": {
			"r": <integer>,
			"g": <integer>,
			"b": <integer>
		},
		// Componente de reflexão difusa
		"kd": {
			"r": <integer>,
			"g": <integer>,
			"b": <integer>
		},
		// Componente de reflexão especular
		"ks": {
			"r": <integer>,
			"g": <integer>,
			"b": <integer>
		},
		// Componente de rugosidade do material
		"alpha": <float>
	}
}
```
