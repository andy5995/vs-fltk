CREATE TABLE `secrets_shared` (
	`id` integer PRIMARY KEY NOT NULL,
	`from_app` integer NOT NULL,
	`with_app` integer NOT NULL,
	`tag` text,
	`notes` text,
	`timestamp_creation` integer NOT NULL,
	`timestamp_expiring` integer NOT NULL,
	FOREIGN KEY (`from_app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade,
	FOREIGN KEY (`with_app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `secretsUnique` ON `secrets_shared` (`from_app`,`with_app`,`tag`);--> statement-breakpoint
CREATE TABLE `secrets` (
	`id` integer PRIMARY KEY NOT NULL,
	`app` integer NOT NULL,
	`tag` text,
	`path` text NOT NULL,
	`value` blob NOT NULL,
	FOREIGN KEY (`app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `secretsUniqueIdx` ON `secrets` (`app`,`tag`,`path`);