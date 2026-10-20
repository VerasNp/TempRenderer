## Cilindro

```json
{
	// Nome do componente objeto
	"name": "<string>",
	"type": "cylinder",
	// Cilindro tem tampa?
	"has_top": <bool>,
	// Cilindro tem base?
	"has_bottom": <bool>,
	// Centro da base
	"base_center": {
		"x": <float>,
		"y": <float>,
		"z": <float>
	},
	// Raio da base
	"base_radius": <float>,
	// Altura do cilindro
	"height": <float>,
	// Direção do cilindro
	"direction": {
		"x": <float>,
		"y": <float>,
		"z": <float>
	},
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
