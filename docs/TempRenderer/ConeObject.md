## Cone

```json
{
	// Nome componente
	"name": "<string>",
	"type": "cone",
	// Centro da base
	"base_center": {
		"x": <float>,
		"y": <float>,
		"z": <float>
	},
	// Raio da base
	"base_radius": <float>,
	// Altura do cone
	"height": <float>,
	// Direção do cone
	"direction": {
		"x": <float>,
		"y": <float>,
		"z": <float>
	},
	// Cone tem base?
	"has_bottom": <bool>,
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
